#include "DllParser.hpp"

#include <iostream>

void TestDllParser()
{
	DllParser dllParser;
	std::string str("MyDLL.dll");

	dllParser.Load(str);

	int a = 5;
	int b = 8;

	auto max = dllParser.ExcecuteFunc<int(const int&, const int&)>("Max", a, b);
	auto add = dllParser.ExcecuteFunc<int(int&, int&)>("Add", a, b);
}

int main(void)
{
	TestDllParser();

	system("pause");
	return 0;
}