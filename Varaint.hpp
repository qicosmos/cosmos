#pragma once

#include <typeindex>
#include <iostream>
#include <type_traits>
#include <functional>
using namespace std;

template <typename T>
struct Function_Traits
	: public Function_Traits<decltype(&T::operator())>
{};

template <typename ClassType, typename ReturnType, typename... Args>
struct Function_Traits<ReturnType(ClassType::*)(Args...) const>
	// we specialize for pointers to member function
{
	enum { arity = sizeof...(Args) };
	// arity is the number of arguments.

	typedef ReturnType result_type;

	template <size_t i>
	struct arg
	{
		typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
	};

	typedef std::function<ReturnType(Args...)> FunType;
	typedef std::tuple<Args...> ArgTupleType;
};

//获取最大的整数
template <size_t arg, size_t... rest>
struct IntegerMax;

template <size_t arg>
struct IntegerMax<arg> : std::integral_constant<size_t, arg>
{
};

template <size_t arg1, size_t arg2, size_t... rest>
struct IntegerMax<arg1, arg2, rest...> : std::integral_constant<size_t, arg1 >= arg2 ? IntegerMax<arg1, rest...>::value :
	IntegerMax<arg2, rest...>::value >
{
};

//获取最大的align
template<typename... Args>
struct MaxAlign : std::integral_constant<int, IntegerMax<std::alignment_of<Args>::value...>::value>{};

//是否包含某个类型
template < typename T, typename... List >
struct Contains;

template < typename T, typename Head, typename... Rest >
struct Contains<T, Head, Rest...>
	: std::conditional< std::is_same<T, Head>::value, std::true_type, Contains<T, Rest... >> ::type{};

template < typename T >
struct Contains<T> : std::false_type{};

template < typename T, typename... List >
struct IndexOf;

//int i = IndexOf<int, double, short, char, int, float>::value;

template < typename T, typename Head, typename... Rest >
struct IndexOf<T, Head, Rest...>
{
	enum{ value = IndexOf<T, Rest...>::value+1 };
};

template < typename T, typename... Rest >
struct IndexOf<T, T, Rest...>
{
	enum{ value = 0 };
};

template < typename T >
struct IndexOf<T>
{
	enum{value = -1};
};

template<int index, typename... Types>
struct At;

// Declaration
template<int index, typename First, typename... Types>
struct At<index, First, Types...> //: At<index - 1, Types...>
{
	using type = typename At<index - 1, Types...>::type;
};

// Specialized
template<typename T, typename... Types>
struct At<0, T, Types...>
{
	using type = T;
};

template<typename... Types>
class Variant
{
	enum
	{
		data_size = IntegerMax<sizeof(Types)...>::value,
		//align_size = IntegerMax<alignof(Types)...>::value
		align_size = MaxAlign<Types...>::value //ctp才有alignof, 为了兼容用此版本
	};
	using data_t = typename std::aligned_storage<data_size, align_size>::type;
public:
	template<int index>
	using IndexType = typename At<index, Types...>::type;

	Variant(void) :m_typeIndex(typeid(void))
	{
	}

	~Variant()
	{
		Destroy(m_typeIndex, &m_data);
	}

	Variant(Variant<Types...>&& old) : m_typeIndex(old.m_typeIndex)
	{
		Move(old.m_typeIndex, &old.m_data, &m_data);
	}

	Variant(const Variant<Types...>& old) : m_typeIndex(old.m_typeIndex)
	{
		Copy(old.m_typeIndex, &old.m_data, &m_data);
	}

	Variant& operator=(const Variant& old)
	{
		Copy(old.m_typeIndex, &old.m_data, &m_data);
		m_typeIndex = old.m_typeIndex;
		return *this;
	}

	Variant& operator=(Variant&& old)
	{
		Move(old.m_typeIndex, &old.m_data, &m_data);
		m_typeIndex = old.m_typeIndex;
		return *this;
	}

	template <class T,
	class = typename std::enable_if<Contains<typename std::remove_reference<T>::type, Types...>::value>::type>
		Variant(T&& value) : m_typeIndex(typeid(void))
	{
			Destroy(m_typeIndex, &m_data);
			typedef typename std::remove_reference<T>::type U;
			new(&m_data) U(std::forward<T>(value));
			m_typeIndex = type_index(typeid(U));
	}

	template<typename T>
	bool Is() const
	{
		return (m_typeIndex == type_index(typeid(T)));
	}

	bool Empty() const
	{
		return m_typeIndex == type_index(typeid(void));
	}

	type_index Type() const
	{
		return m_typeIndex;
	}

	template<typename T>
	typename std::decay<T>::type& Get()
	{
		using U = typename std::decay<T>::type;
		if (!Is<U>())
		{
			cout << typeid(U).name() << " is not defined. " << "current type is " <<
				m_typeIndex.name() << endl;
			throw std::bad_cast();
		}

		return *(U*)(&m_data);
	}

	template<typename T>
	int GetIndexOf()
	{
		return IndexOf<T, Types...>::value;
	}

	template<typename F>
	void Visit(F&& f)
	{
		using T = typename Function_Traits<F>::template arg<0>::type;
		if (Is<T>())
			f(Get<T>());
	}

	template<typename F, typename... Rest>
	void Visit(F&& f, Rest&&... rest)
	{
		using T = typename Function_Traits<F>::template arg<0>::type;
		if (Is<T>())
			Visit(std::forward<F>(f));
		else
			Visit(std::forward<Rest>(rest)...);
	}

	bool operator==(const Variant& rhs) const
	{
		return m_typeIndex == rhs.m_typeIndex;
	}

	bool operator<(const Variant& rhs) const
	{
		return m_typeIndex < rhs.m_typeIndex;
	}

private:
	void Destroy(const type_index& index, void * buf)
	{
		std::initializer_list<int>{(Destroy0<Types>(index, buf), 0)...};
	}

	template<typename T>
	void Destroy0(const type_index& id, void* data)
	{
		if (id == type_index(typeid(T)))
			reinterpret_cast<T*>(data)->~T();
	}

	void Move(const type_index& old_t, void* old_v, void* new_v) 
	{
		std::initializer_list<int>{(Move0<Types>(old_t, old_v, new_v), 0)...};
	}

	template<typename T>
	void Move0(const type_index& old_t, void* old_v, void* new_v)
	{
		if (old_t == type_index(typeid(T)))
			new (new_v)T(std::move(*reinterpret_cast<T*>(old_v)));
	}

	void Copy(const type_index& old_t, void* old_v, void* new_v)
	{
		std::initializer_list<int>{(Copy0<Types>(old_t, old_v, new_v), 0)...};
	}

	template<typename T>
	void Copy0(const type_index& old_t, void* old_v, void* new_v)
	{
		if (old_t == type_index(typeid(T)))
			new (new_v)T(*reinterpret_cast<const T*>(old_v));
	}
private:
	data_t m_data;
	std::type_index m_typeIndex;//类型ID
};
