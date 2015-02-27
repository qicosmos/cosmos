#pragma once
#include "TpIndexs.hpp"

//万能转换法
namespace details
{
	template<typename Last>
	void print_impl(Last&& last)
	{
		std::cout << " " << last << std::endl;
	}

	template<typename Head, typename ... Tail>
	void print_impl(Head&& head, Tail&& ... tail)
	{
		std::cout << " " << head;
		print_impl(tail...);
	}

	//万能转换，将tuple转成Args...
	template<typename ... Args, int ... Indexes >
	void tuple_print_impl(IndexTuple< Indexes... >, tuple<Args...>&& tup)
	{
		print_impl(std::forward<Args>(std::get<Indexes>(tup))...);
	}

} // namespace details


template<typename ... Args>
void print(const tuple<Args...>&  tup)
{
	details::tuple_print_impl(typename details::make_indexes<Args...>::type(), tuple<Args...>(tup));
}

template<typename ... Args>
void print(tuple<Args...>&&  tup)
{
	details::tuple_print_impl(typename details::make_indexes<Args...>::type(), std::forward<tuple<Args...>>(tup));
}

//偏特化+静态方法方式
template<class Tuple, std::size_t N>
struct TuplePrinter {
	static void print(const Tuple& t)
	{
		TuplePrinter<Tuple, N - 1>::print(t);
		std::cout << ", " << std::get<N - 1>(t);
	}
};

template<class Tuple>
struct TuplePrinter<Tuple, 1>{
	static void print(const Tuple& t)
	{
		std::cout << std::get<0>(t);
	}
};

template<class... Args>
void PrintTuple(const std::tuple<Args...>& t)
{
	std::cout << "(";
	TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
	std::cout << ")\n";
}

// ---------- another print method --------------
template<typename T, int... Indexes>
void print_impl2(const T& tup, IndexTuple<Indexes...>)
{
	//// hack 
	//1.逗号操作符，d = (a = b, c); a会被赋值b，d会被赋值c，会按顺序执行逗号前面的表达式，
	//最终的值是逗号操作符的最后一个值，即d=c
	//2.可变参数模板，Args...会生成一个逗号分隔的序列，以满足数组的初始化
	//3.初始化列表，用来初始化数组，并且保证顺序从左到右
	int a[] = { ((std::cout << std::get<Indexes>(tup) << " "), 0)... };
	//(void) garbage; // just to supress warninig -Wunused-variable

	std::cout << std::endl;

}

template<typename... Args>
void print_tuple(const std::tuple<Args...>& tup)
{
	// just for log.
	// if you got an error message: incomplete type, because instantiating template function 'tup' variable is not known at compile - so the const expresion like
	// std::tuple_size<decltype(tup)>::value can't be evalueted at that time.
	//
	// two ways of generating index sequence.
	//typedef typename details::make_indexes2<sizeof... (Args)>::type sequence_type;  
	typedef typename make_indexes<Args...>::type sequence_type;
	print_impl2(tup, sequence_type());
}
