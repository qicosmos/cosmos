#pragma once
// Templates as first-class citizens in C++11
// Example code from http://vitiy.info/templates-as-first-class-citizens-in-cpp11/
// Written by Victor Laskin (victor.laskin@gmail.com)


#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include <tuple>
#include <memory>
#include <sstream>
using namespace std;

#define LOG cout
#define NL endl

//auto tr_map = [](auto fn) {
//	return [=](auto step) {
//		return [=](auto& out, auto&& ...ins) {
//			return step(out, fn(std::forward<decltype(ins)>(ins)...));
//		};
//	};
//};

// apply tuple to function...
template<typename F, size_t... I, typename ... Args>
inline auto tuple_apply(const F& f, const std::index_sequence<I...>&, const std::tuple<Args...>& tp)
{
	return f(std::get<I>(tp)...);
}

template<typename F, typename ... Args>
inline auto tuple_apply(const F& f, const std::tuple<Args...>& tp) -> decltype(f(std::declval<Args>()...))
{
	return tuple_apply(f, std::make_index_sequence<sizeof... (Args)>{}, tp);
}

// end of apply tuple
// universal function / extended function wrapper !
template<typename F, typename Left = std::tuple<>, typename Right = std::tuple<>>
class curry_functor {
private:
	F f_;                            ///< main functor
	Left left_;             ///< curryed arguments
	Right right_;               ///< curryed arguments
	
public:

	curry_functor(F && f) : f_(std::forward<F>(f)), left_(std::tuple<>()), right_(std::tuple<>()) {}
	curry_functor(const F & f, const Left & left, const Right & right) : f_(f), left_(left), right_(right) {}

	template <typename... Args>
	auto operator()(Args... args) const -> decltype(tuple_apply(f_, std::tuple_cat(left_, make_tuple(args...), right_)))
	{
		// execute via tuple
		return tuple_apply(f_, std::tuple_cat(left_, make_tuple(std::forward<Args>(args)...), right_));
	}

	// curry
	template <typename T>
	auto curry_left(T && param) const
	{
		using RealLeft = decltype(std::tuple_cat(left_, std::make_tuple(param)));
		return curry_functor<F, RealLeft, Right>(f_, std::tuple_cat(left_, std::make_tuple(std::forward<T>(param))), right_);
	}

	template <typename T>
	auto curry_right(T && param) const
	{
		using RealRight = decltype(std::tuple_cat(right_, std::make_tuple(param)));
		return curry_functor<F, Left, RealRight>(f_, left_, std::tuple_cat(right_, std::make_tuple(std::forward<T>(param))));
	}
};

template <typename F>
auto fn_to_curry_functor(F && f)
{
	return curry_functor<F>(std::forward<F>(f));
}

// left curry by << operator
template<typename UF, typename Arg>
auto operator<<(const UF & f, Arg && arg) -> decltype(f.template curry_left<Arg>(std::forward<Arg>(arg)))
{
	return f.template curry_left<Arg>(std::forward<Arg>(arg));
}

// right curry by >> operator
template<typename UF, typename Arg>
auto operator>>(const UF & f, Arg && arg) -> decltype(f.template curry_right<Arg>(std::forward<Arg>(arg)))
{
	return f.template curry_right<Arg>(std::forward<Arg>(arg));
}

// OBJECT from TEMPLATE function!
// Template as first-class citizen
#define define_functor(func_name) class tfn_##func_name {\
public: template <typename... Args> auto operator()(Args&&... args) const ->decltype(func_name(std::forward<Args>(args)...))\
{ return func_name(std::forward<Args>(args)...); } }

// tuple concatenation via << operator
template<typename... OldArgs, typename NewArg>
tuple<OldArgs..., NewArg> operator<<(const tuple<OldArgs...> & t, const NewArg& arg)
{
	return std::tuple_cat(t, std::make_tuple(arg));
}

template<typename T, class F>
auto operator|(T&& param, const F& f) -> decltype(f(param))  // typename std::result_of<F(T)>::type
{
	return f(std::forward<T>(param));
}

template <typename... Args>
void print(Args... args)
{
	int a[] = { (LOG << args, 0)... }; LOG << NL;
}

// Return count of elements as templated operator
template <typename T>
int count(const T& container)
{
	return container.size();
}

define_functor(print);
define_functor(count);

/// Universal operators

// MAP
template <typename T, typename... TArgs, template <typename...>class C, typename F>
auto fn_map(const C<T, TArgs...>& container, const F& f) -> C<decltype(f(std::declval<T>()))>
{
	using resultType = decltype(f(std::declval<T>()));
	C<resultType> result;
	for (const auto& item : container)
		result.push_back(f(item));
	return result;
}

// REDUCE (FOLD)
template <typename TResult, typename T, typename... TArgs, template <typename...>class C, typename F>
TResult fn_reduce(const C<T, TArgs...>& container, const TResult& startValue, const F& f)
{
	TResult result = startValue;
	for (const auto& item : container)
		result = f(result, item);
	return result;
}

// FILTER
template <typename T, typename... TArgs, template <typename...>class C, typename F>
C<T, TArgs...> fn_filter(const C<T, TArgs...>& container, const F& f)
{
	C<T, TArgs...> result;
	for (const auto& item : container)
		if (f(item))
			result.push_back(item);
	return result;
}

template <typename T, typename... Args>
T fn_sum(T arg, Args... args)
{
	T result = arg;
	[&result](...) {}((result += args, 0)...);
	return result;
}

#define make_globle_curry_functor(NAME, F) define_functor(F); const auto NAME = fn_to_curry_functor(tfn_##F());

make_globle_curry_functor(map, fn_map);
make_globle_curry_functor(reduce, fn_reduce);
make_globle_curry_functor(filter, fn_filter);
make_globle_curry_functor(sum, fn_sum);

template <typename T, typename TName>
bool is_name_equal_impl(const T& obj, const TName& name)
{
	return (obj->name == name);
}

template <typename T, typename TId>
bool is_id_equal_impl(const T& obj, const TId& id)
{
	return (obj->id == id);
}

template <typename T>
bool is_not_null_impl(const T& t)
{
	return (t != nullptr);
}

template <typename F, typename TKey, typename T, typename... TArgs, template <typename...>class C>
T find_any_impl(const C<T, TArgs...>& container, const F& f, const TKey& key)
{
	for (const auto& item : container)
		if (f(item, key))
			return item;
	return nullptr;
}

make_globle_curry_functor(is_name_equal, is_name_equal_impl);
make_globle_curry_functor(is_id_equal, is_id_equal_impl);
make_globle_curry_functor(is_not_null, is_not_null_impl);
make_globle_curry_functor(find_any, find_any_impl);

// -------------------- chain of functors --------------------->

// The chain of functors ... is actualy just a tuple of functors
template <typename... FNs>
class fn_chain {
private:
	const std::tuple<FNs...> functions_;
	const static size_t TUPLE_SIZE = sizeof...(FNs);
	
	template <std::size_t N, std::size_t I, typename Arg>
	struct final_type : final_type<N - 1, I + 1, decltype(std::get<I>(functions_)(std::declval<Arg>())) > {};

	template <std::size_t I, typename Arg>
	struct final_type<0, I, Arg> {
		using type = decltype(std::get<I>(functions_)(std::declval<Arg>()));
	};

	template <std::size_t I, typename Arg>
	inline typename std::enable_if<I == TUPLE_SIZE -1, decltype(std::get<I>(functions_)(std::declval<Arg>())) >::type
		call(Arg arg) const
	{
		return std::get<I>(functions_)(std::forward<Arg>(arg));
	}

	template <std::size_t I, typename Arg>
	inline typename std::enable_if<I < TUPLE_SIZE -1, typename final_type<TUPLE_SIZE -1 - I, I, Arg>::type >::type
		call(Arg arg) const
	{
		return this->call<I + 1>(std::get<I>(functions_)(std::forward<Arg>(arg)));
	}

public:
	fn_chain() : functions_(std::tuple<>()) {}
	fn_chain(std::tuple<FNs...> functions) : functions_(functions) {}

	// add function into chain
	template< typename F >
	inline auto add(const F& f) const
	{
		return fn_chain<FNs..., F>(std::tuple_cat(functions_, std::make_tuple(f)));
	}

	// call whole functional chain
	template <typename Arg>
	inline auto operator()(Arg arg) const -> decltype(this->call<0, Arg>(arg))

	{
		return call<0>(std::forward<Arg>(arg));
	}
};

// pipe function into functional chain via | operator
template<typename... FNs, typename F>
inline auto operator|(fn_chain<FNs...> && chain, F&& f)
{
	return chain.add(std::forward<F>(f));
}

#define tr fn_chain<>();
// ------------------------------- Monads ---------------------------------->
enum class maybe_state { normal, empty };

template <typename T>
T set_empty() { return T(); }

template<> int set_empty<int>() { return 0; }
template<> string set_empty<string>() { return ""; }

template<typename T>
class maybe {
private:
	const maybe_state state;
	const T x;

	template <typename R>
	maybe<R> fromValue(R&& result) const
	{
		return maybe<R>(std::forward<R>(result));
	}

	template <typename R>
	maybe<std::shared_ptr<R>> fromValue(std::shared_ptr<R>&& result) const
	{
		if (result == nullptr)
			return maybe<std::shared_ptr<R>>();
		else
			return maybe<std::shared_ptr<R>>(std::forward<std::shared_ptr<R>>(result));
	}


public:
	// monadic return
	maybe(T&& x) : x(std::forward<T>(x)), state(maybe_state::normal) {}
	maybe() : x(set_empty<T>()), state(maybe_state::empty) {}

	// monadic bind
	template <typename F>
	auto operator()(F f) const -> maybe<decltype(f(std::declval<T>()))>
	{
		using ResultType = decltype(f(std::declval<T>()));
		if (state == maybe_state::empty)
			return maybe<ResultType>();
		return fromValue(f(x));
	}

	// extract value
	T getOr(T&& anotherValue) const { return (state == maybe_state::empty) ? anotherValue : x; };
};

template<typename T, typename F>
inline auto operator|(maybe<T> && monad, F&& f) -> decltype(monad(f))
{
	return monad(std::forward<F>(f));
}


template<typename T, typename TDefault>
inline T operator||(maybe<T> && monad, TDefault&& t)
{
	return monad.getOr(std::forward<TDefault>(t));
}

template <typename T>
maybe<T> just(T&& t)
{
	return maybe<T>(std::forward<T>(t));
}



string xmlWrapImpl(string name, string item)
{
	return "<" + name + ">" + item + "</" + name + ">";
}

make_globle_curry_functor(xmlWrap, xmlWrapImpl);


/// Simple immutable data
class UserData {
public:
	const int id;
	const string name;
	const int parent;
	UserData(int id, string name, int parent) : id(id), name(name), parent(parent) {}
};

/// Shared pointer to immutable data
using User = std::shared_ptr<UserData>;

template <class T, class... P>
inline auto make(P&&... args) -> T {
	return std::make_shared<typename T::element_type>(std::forward<P>(args)...);
}

