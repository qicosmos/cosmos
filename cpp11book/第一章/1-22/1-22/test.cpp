#include <iostream>
#include <functional>

class A
{
public:
	int i_ = 0;

	void output(int x, int y)
	{
		std::cout << x << " " << y << std::endl;
	}
};

int main(void)
{
	A a;
	std::function<void(int, int)> fr =
		std::bind(&A::output, &a, std::placeholders::_1, std::placeholders::_2);
	fr(1, 2);  //输出 1 2

	std::function<int&(void)> fr_i = std::bind(&A::i_, &a);  //vs13的bug，绑定成员变量要报错
	fr_i() = 123;

	std::cout << a.i_ << std::endl;  //输出 123

	system("pause");
	return 0;
}