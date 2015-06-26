#include "TpZip.hpp"
#include <iostream>

void TupleZip()
{
	auto tp1 = std::make_tuple<int, short, double, char>(1, 2, 2.5, 'a');
	auto tp2 = std::make_tuple<double, short, double, char>(1.5, 2, 2.5, 'z');
	auto mypairs = Zip(tp1, tp2);
}

int main(void)
{
	TupleZip();

	system("pause");
	return 0;
}