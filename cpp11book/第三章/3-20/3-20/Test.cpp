#include <iostream>
#include <tuple>
#include <type_traits>
#include <string>

template <size_t k, typename Tuple>
typename std::enable_if<(k == std::tuple_size<Tuple>::value)>::type
GetArgByIndex(size_t index, Tuple& tp)
{
	throw std::invalid_argument("arg index out of range");
}

template <size_t k = 0, typename Tuple>
typename std::enable_if<(k < std::tuple_size<Tuple>::value)>::type
GetArgByIndex(size_t index, Tuple& tp)
{
	if (k == index)
	{
		std::cout << std::get<k>(tp) << std::endl;
	} 
	else
	{
		GetArgByIndex<k + 1>(index, tp);
	}
}

void TestTuple()
{
	using Tuple = std::tuple<int, double, std::string, int>;
	Tuple tp = std::make_tuple(1, 2, "test", 3);
	const size_t length = std::tuple_size<Tuple>::value;

	//打印每个元素
	for (size_t i = 0; i < length; ++i)
	{
		GetArgByIndex<0>(i, tp);
	}

	GetArgByIndex(4, tp);  //索引超出范围将抛出异常
}

int main(void)
{
	TestTuple();

	system("pause");
	return 0;
}