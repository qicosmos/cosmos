#include <iostream>

class Foo
{
public:
	static const int Number = 0;
	int x;
};

int main(void)
{
	int n = 0;
	volatile const int &x = n;

	decltype(n) a = n;         //a -> int
	decltype(x) b = n;         //b -> const volatile int &

	decltype(Foo::Number) c = 0;  //c -> const int

	system("pause");
	return 0;
}