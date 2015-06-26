#include <iostream>

void func(auto a = 1);  //error:auto不能用于函数参数

struct Foo
{
	auto var1_ = 0;  //error:auto不能用于非静态成员变量
	static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main(void)
{
	int arr[10] = {0};
	auto aa = arr;  //OK: aa -> int *
	auto rr[10] = arr;  //error:auto无法定义数组
	Bar<int> bar;
	Bar<auto> bb = bar;  //error:auto无法推导出模板参数

	system("pause");
	return 0;
}