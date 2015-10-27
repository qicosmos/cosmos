#pragma once
#include <boost/algorithm/minmax_element.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/range.hpp>
#include <boost/range/join.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
using namespace boost;
using namespace boost::adaptors;
#include <numeric>

namespace cosmos
{
	//定义function_traits用于将lambda表达式转换为function
	template <typename Function>
	struct function_traits : public function_traits<decltype(&Function::operator())>
	{};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) const>
	{
		typedef std::function<ReturnType(Args...)> function;
	};

	template <typename Function>
	typename function_traits<Function>::function to_function(Function& lambda)
	{
		return static_cast<typename function_traits<Function>::function>(lambda);
	}

template<typename R>
class LinqCpp
{
public:
	LinqCpp(R& range) : m_linqrange(range)
	{

	}
	typedef typename R::value_type	value_type;

	//过滤操作
	template<typename F>
	auto where(const F& f)->LinqCpp<filtered_range<F,R>>
	{
		return LinqCpp<filtered_range<F, R>>(filter(m_linqrange, f));
	}
	//转换操作
	template<typename F>
	auto select(const F& f)-> LinqCpp<transformed_range<typename function_traits<F>::function, R>>
	{
		auto func = to_function(f);
		return LinqCpp<transformed_range<typename function_traits<F>::function, R>>(boost::adaptors::transform(m_linqrange, func));
	}

	auto begin() const -> decltype(std::begin(boost::declval<const R>()))
	{
		return std::begin(m_linqrange);
	}

	auto end() const -> decltype(std::end(boost::declval<const R>()))
	{
		return std::end(m_linqrange);
	}

	template<typename F>
	auto first(const F& f) -> decltype(std::find_if(begin(), end(), f))
	{
		return std::find_if(begin(), end(), f);
	}

	template<typename F>
	auto last(const F& f) -> decltype(reverse().first(f))
	{
		return reverse().first(f);
	}

	bool empty() const
	{
		return begin() == end();
	}

	template<typename F>
	auto any(const F& f) const -> bool
	{
		return std::any_of(begin(), end(), f);
	}

	template<typename F>
	auto all(const F& f) const -> bool
	{
		return std::all_of(begin(), end(), f);
	}
	
	//遍历操作
	template<typename F>
	void for_each(const F& f) const
	{
		std::for_each(begin(), end(), f);
	}

	//根据判断式判断是否包含
	template<typename F>
	auto contains(const F& f) const -> bool
	{
		return std::find_if(begin(), end(), f);
	}

	//根据function去重
	template<typename F>
	auto distinct(const F& f) const->LinqCpp<decltype(unique(m_linqrange, f))>
	{
		return LinqCpp(unique(m_linqrange, f));
	}

	//简单去重
	auto distinct()  -> LinqCpp<boost::range_detail::uniqued_range<R>>
	{
		return LinqCpp <uniqued_range<R>>(m_linqrange | uniqued);
	}

	//累加器，对每个元素进行一个运算
	template<typename F>
	auto aggregate(const F& f) const -> value_type
	{
		auto it = begin();
		auto value = *it++;
		return std::accumulate(it, end(), std::move(value), f);
	}
	
	//算术运算
	auto sum() const -> value_type
	{
		return aggregate(std::plus<value_type>());
	}

	auto count() const -> decltype(std::distance(begin(), end()))
	{
		return std::distance(begin(), end());
	}

	template<typename F>
	auto count(const F& f) const -> decltype(std::count_if(begin(), end(), f))
	{
		return std::count_if(begin(), end(), f);
	}

	auto average() -> value_type
	{
		return  sum() / count();
	}

	template<typename F>
	auto Min(const F& f) const -> value_type
	{
		return *std::min_element(begin(), end(), f);
	}

	auto Min() const -> value_type
	{
		return *std::min_element(begin(), end());
	}

	template<typename F>
	auto Max(const F& f) const->value_type
	{
		return *std::max_element(begin(), end(), f);
	}

	auto Max() const -> value_type
	{
		return *std::max_element(begin(), end());
	}

	template<typename F>
	auto minmax(const F& f) const->decltype(boost::minmax_element(begin(), end(), f))
	{
		return boost::minmax_element(begin(), end(), f);
	}

	auto minmax() const->decltype(boost::minmax_element(begin(), end()))
	{
		return boost::minmax_element(begin(), end());
	}

	//获取指定索引位置的元素
	template<typename T>
	auto elementat(T index) const->decltype(std::next(begin(), index))
	{
		return std::next(begin(), index);
	}

	//将map中的键放到一个range中
	auto keys() const -> LinqCpp<boost::select_first_range<R>>
	{
		return LinqCpp<boost::select_first_range<R>>(boost::adaptors::keys(m_linqrange));
	}

	////将map中的值放到一个range中
	auto values() const -> LinqCpp<boost::select_second_const_range<R>>
	{
		return LinqCpp<boost::select_second_const_range<R>>(boost::adaptors::values(m_linqrange));
	}

	//反转操作
	auto reverse() ->LinqCpp<boost::reversed_range<R>>
	{
		return LinqCpp <boost::reversed_range<R>>(boost::adaptors::reverse(m_linqrange));
	}

	//获取前面的n个元素
	template<typename T>
	auto take(T n) const->LinqCpp<decltype(slice(m_linqrange, 0, n))>
	{
		return LinqCpp(slice(m_linqrange, 0, n));
	}

	//获取指定范围内的元素
	template<typename T>
	auto take(T start, T end) const->LinqCpp<decltype(slice(m_linqrange, start, end))>
	{
		return LinqCpp(slice(m_linqrange, start, end));
	}

	//将range转换为vector
	vector<value_type> to_vector()
	{
		return vector<value_type>(begin(), end());
	}

	//当条件不满足时返回前面所有的元素
	template<typename F>
	auto takewhile(const F f) const -> LinqCpp<decltype(boost::make_iterator_range(begin(), std::find_if(begin(), end(), f)))>
	{
		return LinqCpp(boost::make_iterator_range(begin(), std::find_if(begin(), end(), f)));
	}

	//获取第n个元素之后的所有元素
	template<typename T>
	auto skip(T n) const->LinqCpp<decltype(boost::make_iterator_range(begin() + n, end()))>
	{
		return LinqCpp(boost::make_iterator_range(begin() + n, end()));
	}

	//当条件不满足时，获取后面所有的元素
	template<typename F>
	auto skipwhile(const F& f) const -> LinqCpp<iterator_range < decltype(boost::make_iterator_range(std::find_if_not(begin(), end(), f), end()))>>
	{
		return LinqCpp(boost::make_iterator_range(std::find_if_not(begin(), end(), f), end()));
	}

	//按步长挑选元素组成新集合
	template<typename T>
	auto step(T n) ->decltype(stride(m_linqrange, n))
	{
		return stride(m_linqrange, n);
	}

	//直接将指针或者智能指针指向的内容组成新集合
	auto indirect()->LinqCpp<boost::indirected_range <R>>
	{
		return LinqCpp<boost::indirected_range<R>>(boost::adaptors::indirect(m_linqrange));
	}
	//连接操作
	template<typename R2>
	auto concat(const R2& other) ->LinqCpp<joined_range<R, const R2>>
	{
		return LinqCpp<joined_range<R, const R2>>(boost::join(m_linqrange, other));
	}

	//排除操作
	template<typename R2>
	void except(const R2& other, std::vector<value_type>& resultVector)
	{
		std::set_difference(begin(), end(), std::begin(other), std::end(other), back_inserter(resultVector));
	}

	//包含操作
	template<typename R2>
	bool includes(const R2& other) const
	{
		return std::includes(begin(), end(), std::begin(other), std::end(other));
	}

	//分组操作
	template<typename Fn>
	multimap<typename std::result_of<Fn(value_type)>::type, value_type> groupby(const Fn& f)  
	{
		typedef  decltype(std::declval<Fn>()(std::declval <value_type>())) keytype;
		multimap<keytype, value_type> mymap;
		std::for_each(begin(), end(), [&mymap, &f](value_type item)
		{
			mymap.insert(make_pair(f(item), item));
		});
		return mymap;
	}

	//允许指定键和值函数的分组操作
	template<typename KeyFn, typename ValueFn>
	multimap<typename std::result_of<KeyFn(value_type)>::type, typename std::result_of<ValueFn(value_type)>::type> groupby(const KeyFn& fnk, const ValueFn& fnv)  
	{
		typedef  typename std::result_of<KeyFn(value_type)>::type keytype;
		typedef  typename std::result_of<ValueFn(value_type)>::type valype;

		multimap<keytype, valype> mymap;
		std::for_each(begin(), end(), [&mymap, &fnk, &fnv](value_type item)
		{
			keytype key = fnk(item);
			valype val = fnv(item);
			mymap.insert(make_pair(key, val));
		});
		return mymap;
	}

	//转换操作
	template<typename T>
	auto cast()->LinqCpp<boost::transformed_range<std::function < T(value_type)>, R>>
	{
		std::function < T(value_type)> f = [](value_type item){return static_cast<T>(item); };
		return LinqCpp<transformed_range<std::function < T(value_type)>, R>>(select(f));
	}

	//判断操作
	template<typename R2>
	bool equals(const LinqCpp<R2>& other) const
	{
		return count() == other.count() && std::equal(begin(), end(), other.begin());
	}

	template<typename R2, typename F>
	bool equals(const LinqCpp<R2>& other, const F& f) const
	{
		return count() == other.count() && std::equal(begin(), end(), other.begin(), f);
	}

	template<typename R2>
	bool operator==(const LinqCpp<R2>& other) const
	{
		return equals(other);
	}

	template<typename R2>
	bool operator!=(const LinqCpp<R2>& other) const
	{
		return !(*this == other);
	}
private:
	R m_linqrange;
};

//简化range的声明
template<template<typename T> class IteratorRange, typename R>
using Range = IteratorRange<decltype(std::begin(std::declval<R>()))>;

template<typename R>
using iterator_range = Range<boost::iterator_range, R>;

//简化定义LinqCpp的辅助函数
template<typename R>
LinqCpp<iterator_range<R>> from(const R& range)
{
	return LinqCpp<iterator_range<R>>(iterator_range<R>(range));
}

//合并range
template <typename... T>
auto zip(const T&... containers) -> boost::iterator_range<boost::zip_iterator<decltype(boost::make_tuple(std::begin(containers)...))>>
{
	auto zip_begin = boost::make_zip_iterator(boost::make_tuple(std::begin(containers)...));
	auto zip_end = boost::make_zip_iterator(boost::make_tuple(std::end(containers)...));
	return boost::make_iterator_range(zip_begin, zip_end);
}
}

/*test code
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
#include "LinqCpp.hpp"
void TestLinqCpp()
{
	using namespace cosmos;
	vector<int>v = { 0, 1, 2, 3, 4, 5, 3, 6};
	vector<string> strv = { "a", "b", "c" };
	map<int, int> mymap = { {1, 3}, { 2, 1 }, { 3, 2 } };
	
	//聚合
	string alstr = cosmos::from(strv).aggregate([](const string& str1, const string& str2){
		return str1 + str2;
	});
	//将输出abc

	//distinct
	sort(v); //distinct之前要先排序否则不对
	auto result = from(v).where([](int x){return x % 2 != 0; }).distinct().to_vector();
	//将输出{0,1,2,3,4,5,6}的vector<int>集合。

	//计算
	auto ct = from(v).count();
	auto sm = from(v).sum();
	auto av = from(v).average();
	auto min = from(v).Min();
	auto max = from(v).Max();

	//元素操作符
	auto elm = from(v).elementat(2);
	//将输出2

	//反转
	auto rv = from(v).reverse();

	//区间操作
	auto tk = from(v).take(3).to_vector();
	auto tkw = from(v).takewhile([](int a){return a > 3; }); //遇到不满足条件的就返回，从开始到终止时的范围
	
	auto skp = from(v).skip(3);
	auto skpw = from(v).skipwhile([](int a){return a < 3; }).to_vector(); //不满足条件到end范围

	auto step = from(v).step(2); //以步长为2组成新序列

	//取map中的键组成新的序列
	auto keys = from(mymap).keys();

	//取map中的值组成新的序列
	auto values = from(mymap).values();
}

*/
