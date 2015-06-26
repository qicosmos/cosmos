#include "Variant.hpp"
#include <iostream>
#include <string>

void Test()
{
	typedef Variant<int, double, std::string, int> cv;

	//根据index获取类型
	std::cout << typeid(cv::IndexType<1>).name() << std::endl;

	//根据类型获取索引
	cv v = 10;
	int i = v.GetIndexOf<std::string>(); 
	std::cout << "i = " << i << std::endl;

	//通过一组lambda访问vairant
	v.Visit([&](double i){std::cout << "double: " << i << std::endl; },
		[&](short i){std::cout << "short: " << i << std::endl; },
		[](int i){std::cout << "int: " << i << std::endl; },
		[](std::string i){std::cout << "std::string: " << i << std::endl; } );

	bool emp1 = v.Empty();
	std::cout << v.Type().name() << std::endl;
}

int main(void)
{
	Test();

	system("pause");
	return 0;
}