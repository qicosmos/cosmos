#include <iostream>
#include <tuple>
#include <string>

template <typename Arg>
void GetArgByIndex(int index, std::tuple<Arg>& tp)
{
	std::cout << std::get<0>(tp) << std::endl;
}

template <typename Arg, typename... Args>
void GetArgByIndex(int index, std::tuple<Arg, Args...>& tp)
{
	if (index < 0 || index >= std::tuple_size<std::tuple<Arg, Args...>>::value)
	{
		throw std::invalid_argument("index is not valid");
	}

	if (index > 0)
	{
		GetArgByIndex(index - 1, (std::tuple<Args...>&) tp);
	} 
	else
	{
		std::cout << std::get<0>(tp) << std::endl;
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
		GetArgByIndex(i, tp);
	}

	GetArgByIndex(4, tp);  //索引超出范围将抛出异常
}


int main(void)
{
	TestTuple();

	system("pause");
	return 0;
}