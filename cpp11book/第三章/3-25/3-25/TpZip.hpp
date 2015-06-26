#pragma once
#include <utility>
#include <tuple>

//将两个tuple合起来，前一个tuple中的每个元素为key，后一个tuple中的每个元素为value，组成一个pair集合
namespace details
{
	template <int...>
	struct IndexTuple {};

	template <int N, int...Indexs>
	struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexs...>{};

	template <int...Indexs>
	struct MakeIndexes<0, Indexs...>
	{
		typedef IndexTuple<Indexs...> type;
	};

	template<std::size_t N, typename T1, typename T2>
	using pair_type = std::pair<typename std::tuple_element<N, T1>::type, typename std::tuple_element<N, T2>::type>;

	template<std::size_t N, typename T1, typename T2>
	pair_type<N, T1, T2> pair(const T1& tup1, const T2& tup2)
	{
		//
		return std::make_pair(std::get<N>(tup1), std::get<N>(tup2));
	}

	template<int... Indexes, typename T1, typename T2>
	auto pairs_helper(IndexTuple<Indexes...>, const T1& tup1, const T2& tup2) -> decltype(std::make_tuple(pair<Indexes>(tup1, tup2)...))
	{
		return std::make_tuple(pair<Indexes>(tup1, tup2)...);
	}

} // namespace details

template<typename Tuple1, typename Tuple2>
auto Zip(Tuple1 tup1, Tuple2 tup2) -> decltype(details::pairs_helper(
	typename details::MakeIndexes<std::tuple_size<Tuple1>::value>::type(), tup1, tup2))
{
	static_assert(std::tuple_size<Tuple1>::value == std::tuple_size<Tuple2>::value,
		"tuples should be the same size.");
	return details::pairs_helper(typename
		details::MakeIndexes<std::tuple_size<Tuple1>::value>::type(), tup1, tup2);
}
