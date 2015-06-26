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

template <typename T>
void Print(T t)
{
	std::cout << t << std::endl;
}

template <typename T, typename ...Args>
void Print(T t, Args...args)
{
	std::cout << t << std::endl;
	Print(args...);
}

template <typename Tuple, int...Indexs>
void Transform(IndexTuple<Indexs...>& in, Tuple& tp)
{
	Print(std::get<Indexs>(tp)...);
}

int main(void)
{
	using Tuple = std::tuple<int, double>;
	Tuple tp = std::make_tuple(1, 2);
	Transform(MakeIndexes<std::tuple_size<Tuple>::value>::type(), tp);

	system("pause");
	return 0;
}