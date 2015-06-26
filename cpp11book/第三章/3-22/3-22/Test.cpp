#include "TpForeach.hpp"
#include <iostream>

struct Functor
{
	template <typename T>
	void operator()(T& t) const
	{
		//t.doSomething();
		std::cout << t << std::endl;
	}
};

void TestTupleForeach()
{
	tp_for_each(Functor(), std::make_tuple<int, double>(1, 2.5));
}

int main(void)
{
	TestTupleForeach();

	system("pause");
	return 0;
}