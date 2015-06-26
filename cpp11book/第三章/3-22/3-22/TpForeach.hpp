#pragma once
#include "TpIndexs.hpp"

#include <tuple>

namespace details {

	template<typename Func, typename Last>
	void for_each_impl(Func&& f, Last&& last)
	{
		f(last);
	}

	template<typename Func, typename First, typename ... Rest>
	void for_each_impl(Func&& f, First&& first, Rest&&...rest)
	{
		f(first);
		for_each_impl(std::forward<Func>(f), rest...);
	}

	template<typename Func, int ... Indexes, typename ... Args>
	void for_each_helper(Func&& f, IndexTuple<Indexes...>, std::tuple<Args...>&& tup)
	{
		for_each_impl(std::forward<Func>(f), std::forward<Args>(std::get<Indexes>(tup))...);
	}

} // namespace details 

template<typename Func, typename Tuple>
void tp_for_each(Func&& f, Tuple& tup)
{
	using namespace details;
	for_each_helper(std::forward<Func>(f), typename make_indexes<
		std::tuple_size<Tuple>::value>::type(), tup);
}

template<typename Func, typename Tuple>
void tp_for_each(Func&& f, Tuple&& tup)
{
	using namespace details;
	for_each_helper(std::forward<Func>(f), typename make_indexes<
		std::tuple_size<Tuple>::value>::type(),
		std::forward<Tuple>(tup));
}

