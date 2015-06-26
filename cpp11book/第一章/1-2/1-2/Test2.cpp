#include <iostream>

class Foo
{
public:
	static int get(void)
	{
		return 0;
	}
};

class Bar
{
public:
	static const char* get(void)
	{
		return "0";
	}
};

template <class A>
void func(void)
{
	auto val = A::get();

	//...
}

int main(void)
{
	func<Foo>();
	func<Bar>();

	system("pause");
	return 0;
}