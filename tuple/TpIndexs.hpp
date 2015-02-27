#pragma once
#include <tuple>
using std::tuple;
using std::forward;
using std::get;

//tuple参数的索引序列
template<int...>
struct IndexTuple{};

//forward declare
template<int I, typename IndexTuple, typename... Types>
struct make_indexes_impl;

//declare
template<int I, int... Indexes, typename T, typename... Types>
struct make_indexes_impl<I, IndexTuple<Indexes...>, T, Types...>
{
	using type = typename make_indexes_impl<I + 1, IndexTuple<Indexes..., I>, Types...>::type;
};

//terminate
template<int I, int... Indexes>
struct make_indexes_impl<I, IndexTuple<Indexes...>>
{
	using type = IndexTuple<Indexes...>;
};

//type trait
template<typename ... Types>
struct make_indexes : make_indexes_impl<0, IndexTuple<>, Types...>
{};

/*********************************************/
template<int I, typename IndexTuple, typename... Types>
struct make_indexes_reverse_impl;

//declare
template<int I, int... Indexes, typename T, typename... Types>
struct make_indexes_reverse_impl<I, IndexTuple<Indexes...>, T, Types...>
{
	using type = typename make_indexes_reverse_impl<I-1, IndexTuple<Indexes..., I - 1>, Types...>::type;
};

//terminate
template<int I, int... Indexes>
struct make_indexes_reverse_impl<I, IndexTuple<Indexes...>>
{
	using type = IndexTuple<Indexes...>;
};

//type trait
template<typename ... Types>
struct make_reverse_indexes : make_indexes_reverse_impl<sizeof...(Types), IndexTuple<>, Types...>
{};
/*********************************************/

// ---- another method to generate sequence/indexes from tuple arguments.
template<int N, int... Indexes>
struct make_indexes2 : make_indexes2<N - 1, N - 1, Indexes...> {};

// template specialization - termination condition
template<int... Indexes>
struct make_indexes2<0, Indexes...>
{
	typedef IndexTuple<Indexes...> type;
};

/***********************************************/

template< int end, int cur, int... Indexes >
struct make_indexes3 : make_indexes3< end, cur + 1, Indexes..., cur >{};

// cur == end; the list has been built.
template< int end, int... Indexes >
struct make_indexes3< end, end, Indexes... >
{
	typedef IndexTuple<Indexes...> type;
};

/***********************************************/
