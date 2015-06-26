#include <iostream>

int i_arr[3] = { 1, 2, 3 };  //普通数组

struct A
{
	int x;
	struct B
	{
		int i;
		int j;
	} b;
} a = { 1, {2, 3} };  //POD类型

int i = 0;

class Foo
{
public:
	Foo(int){}
} foo = 123;

int j(0);

Foo bar(123);

int main(void)
{

	system("pause");
	return 0;
}