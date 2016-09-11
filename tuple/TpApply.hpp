#pragma once
#include "TpIndexs.hpp"

template<class F, class... Args, int... Indexes >
typename std::result_of<F(Args...)>::type apply_helper(F&& pf, IndexTuple< Indexes... >&& in, tuple<Args...>&& tup)
{
	return forward<F>(pf)(forward<Args>(get<Indexes>(tup))...);
}

template<class F, class ... Args>
typename std::result_of<F(Args...)>::type apply(F&& pf, const tuple<Args...>&  tup)
{
	return apply_helper(forward<F>(pf), typename make_indexes<Args...>::type(), tuple<Args...>(tup));
}

template<class F, class ... Args>
typename std::result_of<F(Args...)>::type apply(F&& pf, tuple<Args...>&&  tup)
{
	return apply_helper(forward<F>(pf), typename make_indexes<Args...>::type(), forward<tuple<Args...>>(tup));
}

///***************
//recurse
//<<1, <0>, Args...>type>
//<<2, <1>, Args...>type>
//<<3, <2>, Args...>type>
//<<4, <3>, Args...>type>
//terminate
//<<5, <>>type> 
//==> evalue
//==> result index sequence: <3, 2, 1, 0>
///

template<class F, class ... Args>
typename std::result_of<F(Args...)>::type apply2(F&& pf, const tuple<Args...>&  tup)
{
	return apply_helper(forward<F>(pf), typename make_indexes2<sizeof ...(Args)>::type(), tuple<Args...>(tup));
}

template<class F, class ... Args>
typename std::result_of<F(Args...)>::type apply2(F&& pf, tuple<Args...>&&  tup)
{
	return apply_helper(forward<F>(pf), typename make_indexes2<sizeof ...(Args)>::type(), forward<tuple<Args...>>(tup));
}

//template< int ... > struct make_indexes3;
//
//// Increment cur until cur == end.
//// Recurse, adding cur to i...
//template< int end, int cur, int... Indexes >
//struct make_indexes3< end, cur, Indexes... >: make_indexes3< end, cur + 1, Indexes..., cur >
//{};

template<class F, class ... Args>
typename std::result_of<F(Args...)>::type apply3(F&& pf, const tuple<Args...>&  tup)
{
	return apply_helper(forward<F>(pf), typename make_indexes3<sizeof ...(Args), 0>::type(), tuple<Args...>(tup));
}

template<class F, class ... Args>
typename std::result_of<F(Args...)>::type apply3(F&& pf, tuple<Args...>&&  tup)
{
	return apply_helper(forward<F>(pf), typename make_indexes3<sizeof ...(Args), 0>::type(), forward<tuple<Args...>>(tup));
}

//根据值获取tuple index
namespace detail
{
	template<int I, typename T, typename... Args>
	struct find_index
	{
		static int call(std::tuple<Args...> const& t, T&& val)
		{
			return (std::get<I - 1>(t) == val) ? I - 1 :
				find_index<I - 1, T, Args...>::call(t, std::forward<T>(val));
		}
	};

	template<typename T, typename... Args>
	struct find_index<0, T, Args...>
	{
		static int call(std::tuple<Args...> const& t, T&& val)
		{
			return (std::get<0>(t) == val) ? 0 : -1;
		}
	};
}

template<typename T, typename... Args>
int find_index(std::tuple<Args...> const& t, T&& val)
{
	return detail::find_index<sizeof...(Args) -1, T, Args...>::
		call(t, std::forward<T>(val));
}
