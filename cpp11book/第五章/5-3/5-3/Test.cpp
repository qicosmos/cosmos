#include <iostream>
#include <thread>
#include <mutex>

struct Complex
{
	std::recursive_mutex mutex;
	int i;

	Complex() : i(0){}

	void mul(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i *= x;
	}

	void div(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i /= x;
	}

	void both(int x, int y)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		mul(x);
		div(y);
	}
};

int main(void)
{
	Complex complex;

	complex.both(32, 23);  //因为同一线程可以多次获取同一互斥量，不会发生死锁

	system("pause");
	return 0;
}