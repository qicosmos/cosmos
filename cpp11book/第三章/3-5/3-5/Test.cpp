#include "optional.hpp"
#include <iostream>
#include <string>
#include <map>

struct MyStruct
{
	MyStruct(){}
	MyStruct(int a, int b) : m_a(a), m_b(b){}
	int m_a;
	int m_b;
};

void TestOptional()
{
	Optional<std::string> a("ok");
	Optional<std::string> b("ok");
	Optional<std::string> c("aa");

	c = a;

	Optional<MyStruct> op;
	op.emplace(1, 2);
	MyStruct t; 

	if (op)  //判断op是否被初始化
	{
		t = *op;
	}

	op.emplace(3, 4);
	t = *op;
}

int main(void)
{
	TestOptional();

	system("pause");
	return 0;
}