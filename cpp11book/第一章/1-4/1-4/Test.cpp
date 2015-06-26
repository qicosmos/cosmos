#include <iostream>

class Foo{};

int& func_int_r(void);    //左值（lvalue，可简单理解为可寻址值）
int&& func_int_rr(void);  //x值（xvalue，右值引用本身是一个xvalue）
int func_int(void);       //纯右值（pvalue）

const int& func_cint_r(void);         //左值
const int&& func_cint_rr(void);       //x值
const int func_cint(void);            //纯右值

const Foo func_cfoo(void);  //纯右值

int main(void)
{
	int x = 0;

	decltype(func_int_r()) a1 = x;    //a1 -> int&
	decltype(func_int_rr()) b1 = 0;   //b1 -> int&&
	decltype(func_int()) c1 = 0;      //c1 -> int

	decltype(func_cint_r()) a2 = x;    //a2 -> const int&
	decltype(func_cint_rr()) b2 = 0;   //b2 -> const int&&
	decltype(func_cint()) c2 = 0;      //c2 -> int

	decltype(func_cfoo()) ff = Foo();  //ff -> Foo

	system("pause");
	return 0;
}