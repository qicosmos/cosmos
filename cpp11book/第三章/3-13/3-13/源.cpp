#include "Any.hpp"

#include <iostream>
#include <string>

void TestAny()
{
	Any n;
	auto r = n.IsNull();//true
	std::string s1 = "hello";
	n = s1;
	n = "world";
	n.AnyCast<int>();  //can not cast int to string
	Any n1 = 1;
	n1.Is<int>(); //true
}


int main(void)
{
	TestAny();

	system("pause");
	return 0;
}