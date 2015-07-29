#include <iostream>

void PrintT(int& t)
{
	std::cout << "lvalue" << std::endl;
}

template <typename T>
void PrintT(T&& t)
{
	std::cout << "rvalue" << std::endl;
}

template <typename T>
void TestForward(T&& v)
{
	PrintT(v);
	PrintT(std::forward<T>(v));
	PrintT(std::move(v));
}

void Test()
{
	TestForward(1);
	int x = 1;
	TestForward(x);
	TestForward(std::forward<int>(x));
}

int main(void)
{
	Test();

	system("pause");
	return 0;
}
