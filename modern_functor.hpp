#pragma once
// Templates as first-class citizens in C++11
// inspired by http://vitiy.info/templates-as-first-class-citizens-in-cpp11/
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
template<typename F, typename Before = std::tuple<>, typename After = std::tuple<>>
class curry_functor {
private:
	F f_;                            ///< main functor
	Before before_;             ///< curryed arguments
	After after_;               ///< curryed arguments
	
public:

	curry_functor(F && f) : f_(std::forward<F>(f)), before_(std::tuple<>()), after_(std::tuple<>()) {}
	curry_functor(const F & f, const Before & before, const After & after) : f_(f), before_(before), after_(after) {}

	template <typename... Args>
	auto operator()(Args... args) const -> decltype(tuple_apply(f_, std::tuple_cat(before_, make_tuple(args...), after_)))
	{
		// execute via tuple
		return tuple_apply(f_, std::tuple_cat(before_, make_tuple(std::forward<Args>(args)...), after_));
	}

	// curry
	template <typename T>
	auto curry_before(T && param) const
	{
		using RealBefore = decltype(std::tuple_cat(before_, std::make_tuple(param)));
		return curry_functor<F, RealBefore, After>(f_, std::tuple_cat(before_, std::make_tuple(std::forward<T>(param))), after_);
	}

	template <typename T>
	auto curry_after(T && param) const
	{
		using RealAfter = decltype(std::tuple_cat(after_, std::make_tuple(param)));
		return curry_functor<F, Before, RealAfter>(f_, before_, std::tuple_cat(after_, std::make_tuple(std::forward<T>(param))));
	}
};

template <typename F>
auto fn_to_curry_functor(F && f)
{
	return curry_functor<F>(std::forward<F>(f));
}

// before curry by << operator
template<typename UF, typename Arg>
auto operator<<(const UF & f, Arg && arg) -> decltype(f.template curry_before<Arg>(std::forward<Arg>(arg)))
{
	return f.template curry_before<Arg>(std::forward<Arg>(arg));
}

// after curry by >> operator
template<typename UF, typename Arg>
auto operator>>(const UF & f, Arg && arg) -> decltype(f.template curry_after<Arg>(std::forward<Arg>(arg)))
{
	return f.template curry_after<Arg>(std::forward<Arg>(arg));
}

// OBJECT from TEMPLATE function!
// Template as first-class citizen
#define define_functor_type(func_name) class tfn_##func_name {\
public: template <typename... Args> auto operator()(Args&&... args) const ->decltype(func_name(std::forward<Args>(args)...))\
{ return func_name(std::forward<Args>(args)...); } }

// tuple concatenation via << operator
template<typename... OldArgs, typename NewArg>
tuple<OldArgs..., NewArg> operator<<(const tuple<OldArgs...> & t, const NewArg& arg)
{
	return std::tuple_cat(t, std::make_tuple(arg));
}

template<typename T, class F>
auto operator|(T&& param, const F& f) -> decltype(f(std::forward<T>(param)))  // typename std::result_of<F(T)>::type
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

define_functor_type(print);
define_functor_type(count);

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

#define make_globle_curry_functor(NAME, F) define_functor_type(F); const auto NAME = fn_to_curry_functor(tfn_##F());

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
/*	
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
*/
	template<typename Arg, std::size_t I>
	auto call_impl(Arg&& arg, const std::index_sequence<I>&) const ->decltype(std::get<I>(functions_)(std::forward<Arg>(arg)))
	{
		return std::get<I>(functions_)(std::forward<Arg>(arg));
	}

	template<typename Arg, std::size_t I, std::size_t... Is>
	auto call_impl(Arg&& arg, const std::index_sequence<I, Is...>&) const ->decltype(call_impl(std::get<I>(functions_)(std::forward<Arg>(arg)), std::index_sequence<Is...>{}))
	{
		return call_impl(std::get<I>(functions_)(std::forward<Arg>(arg)), std::index_sequence<Is...>{});
	}

	template<typename Arg>
	auto call(Arg&& arg) const-> decltype(call_impl(std::forward<Arg>(arg), std::make_index_sequence<sizeof...(FNs)>{}))
	{
		return call_impl(std::forward<Arg>(arg), std::make_index_sequence<sizeof...(FNs)>{});
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
	//template <typename Arg>
	//inline auto operator()(Arg&& arg) const -> decltype(this->call<0, Arg>(std::forward<Arg>(arg)))
	//{
	//	return call<0>(std::forward<Arg>(arg));
	//}

	template <typename Arg>
	inline auto operator()(Arg&& arg) const -> decltype(call(std::forward<Arg>(arg)))
	{
		return call(std::forward<Arg>(arg));
	}
};

// pipe function into functional chain via | operator
template<typename... FNs, typename F>
inline auto operator|(fn_chain<FNs...> && chain, F&& f)
{
	return chain.add(std::forward<F>(f));
}

#define tfn_chain fn_chain<>()
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

/*test code
int fn_add_one(int a) { return 1 + a; }
int add1(int a) { return 1 + 1; }
define_functor_type(add1);
make_globle_curry_functor(addone, fn_add_one);

void test_pipeline()
{
	//test define functor, just define functor from function, the prefix is tfn_
	tfn_add1 ad;  //define a functor

	//test operator|, just surpport lambda, functor, std::function, return the result
	auto add_one = [](auto a) { return 1 + a; };
	auto result = 2 | add_one;
	result = 2 | addone;
	result = 2 | ad;
	cout << result << endl;
	std::function<int(int)> func = std::bind(fn_add_one, std::placeholders::_1);
	result = 2 | func;
	result = 2 | std::bind(fn_add_one, std::placeholders::_1);

	//test tuple_apply
	auto f = [](int x, int y, int z) { return x + y - z; };
	auto params = make_tuple(1, 2, 3);
	auto res = tuple_apply(f, params);

	//test curry, left args, right
	auto curry_fn1 = fn_to_curry_functor(f);
	result = 3 | curry_fn1 << 1 >> 2; //1, 3, 2
	result = (curry_fn1 << 1 >> 2 << 3)();

	//test map reduce
	vector<string> slist = { "one", "two", "three" };

	slist | (map >> [](auto s) { return s.size(); }) 
		| (reduce >> 0 >> [](auto a, auto b) { return a + b; }) 
		| [](auto a) { cout << a << endl; };

	//test chain, lazy eval
	auto chain = tfn_chain | (map >> [](auto s) { return s.size(); })
		| (reduce >> 0 >> [](auto a, auto b) { return a + b; }) 
		| ([](int a) { std::cout << a << std::endl; });

	slist | chain;

	//test aop
	person p = { 20, "tom" };
	std::function<person(int)> func1 = std::bind(&person::get_person_by_id, &p, std::placeholders::_1);
	auto testfn = fn_to_curry_functor(func1);

	auto aspect = tfn_chain | ([](int id) { cout << "before"; return id + 1; }) 
		| func1 
		| ([](const person& p) { cout << "after" << endl; });

	aspect(1);
}

void test()
{
	using namespace std;
	
	tfn_print print;
	print(5);
	print("hello");

	// let's call function by providing tuple

	auto f = [](int x, int y, int z) { return x + y - z; };
	auto params = make_tuple(1, 2, 3);
	auto res = tuple_apply(f, params);
	print(res);


	// combine tuple using overloaded operators and apply function
	auto list = make_tuple(1, 4);
	auto res2 = tuple_apply(f, list << 4);
	print(res2);

	// ok, now i want piped templates
	tfn_count count;
	vector<string> slist = { "one", "two", "three" };
	slist | count | print;


	// piping....
	auto ucount = fn_to_curry_functor(count);
	auto uprint = fn_to_curry_functor(print);
	//slist | ucount | print;

	// currying....
	auto uf = fn_to_curry_functor(f);
	auto uf1 = uf << 1;
	auto uf2 = uf1 << 2 << 5;
	uf2() | print;
	auto ff1 = uf << 4 << 6 << 1;
	auto ff2 = uf >> 4 >> 6 >> 1;
	1 | (uf << 4 << 6) | print; // 4+6-1 = 9

	3 | (uf >> 6 >> 7) | print; // 3+6-7 = 2


								// count sum length of all strings in the list

	slist | (map >> count) | (reduce >> 0 >> sum) | (uprint << "Total: " >> " chars");

	slist | (reduce >> string("") >> sum) | (uprint << "All: ");

	vector<int>{1, 2, 3} | (reduce >> 0 >> sum) | (uprint << "Sum: ");

	// Some real objects...
	vector<User> users{ make<User>(1, "John", 0), make<User>(2, "Bob", 1), make<User>(3, "Max", 1) };

	users | (filter >> (is_name_equal >> "Bob")) | ucount | uprint;

	vector<int>{1, 2, 6} | (filter >> [](auto i) { return i % 2 == 0; }) | ucount | uprint;

	// Produce XML
	users | map >> [](User u) { return u->name; } | map >> (xmlWrap << "name") | reduce >> string("") >> sum | xmlWrap << "users" | print;

	// Use functional chain:
	auto f1 = [](int x) { return x + 3; };
	auto f2 = [](int x) { return x * 2; };
	auto f3 = [](int x) { return (double)x / 2.0; };
	auto f4 = [](double x) { std::stringstream ss; ss << x; return ss.str(); };
	auto f5 = [](string s) { return "Result: " + s; };
	auto testChain = fn_chain<>() | f1 | f2 | f3 | f4 | f5;
	testChain(3) | print;

	auto countUsers = fn_chain<>() | (map >> (find_any << users << is_id_equal)) | (filter >> is_not_null) | ucount;
	vector<int>{1, 2, 6} | countUsers | (uprint << "count of users: ");


	// Ok, pipe through monadic execution!

	maybe<int>(2) | (find_any << users << is_id_equal) | [](User u) { return u->name; } | [](string s) { LOG << s << NL; return s; };

	(maybe<int>(6) | (find_any << users << is_id_equal) | [](User u) { return u->name; }).getOr("Not found") | (uprint << "Found user: ");

	just(vector<int>{1, 2, 6}) | countUsers | [&](int count) { count | (uprint << "Count: "); return count; };

	(just(vector<int>{1, 2, 6}) | countUsers || -1) | (uprint << "Count:");
	//auto fn1 = fn_chain<>() |filter([](int x) { return (x % 2 == 0); }) | map([](auto x) { return x + 1; });
	//auto enumerateStrings = (tr | tenumerate() | tmap([](int n, string s) { return s + " " +
	//	std::to_string(n); }))(output_rf);
	//auto result = fn_tr_reduce(vector<string>(), enumerateStrings, vector<string>{"a", "b", "c"});

	//vector<int> input{ 1,2,3,4,5,6,7 };
	//auto result = input | into_vector << (tr | tfilter([](int x) { return (x % 2 == 0); }) | tmap([](auto x) { return x + 1; }));
}
*/
