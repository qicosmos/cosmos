#include <iostream>
#include <tuple>

template <int...>
struct IndexTuple {};

template <int N, int...Indexs>
struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexs...>{};

template <int...Indexs>
struct MakeIndexes<0, Indexs...>
{
	typedef IndexTuple<Indexs...> type;
};

template <typename F, typename Tuple, int...Indexes>
auto apply(F&& f, IndexTuple<Indexes...>&& in, Tuple&& tp) ->
decltype(std::forward<F>(f)(std::get<Indexes>(tp)...))
{
	std::forward<F>(f)(std::get<Indexes>(tp)...);
}

void TestF(int a, double b)
{
	std::cout << a + b << std::endl;
}

int main(void)
{
	apply(TestF, MakeIndexes<2>::type(), std::make_tuple(1, 2));  //Êä³ö : 3

	system("pause");
	return 0;
}