#pragma once

template <int...>
struct IndexTuple {};

template <int N, int...Indexs>
struct make_indexes : make_indexes<N - 1, N - 1, Indexs...>{};

template <int...Indexs>
struct make_indexes<0, Indexs...>
{
	typedef IndexTuple<Indexs...> type;
};