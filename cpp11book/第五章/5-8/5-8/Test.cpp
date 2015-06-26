#include <iostream>
#include <thread>
#include <atomic>

struct Counter
{
	std::atomic<int> value = 0;

	void increment()
	{
		++value;
	}

	void decrement()
	{
		--value;
	}

	int get()
	{
		return value;
	}
};

Counter g_counter;

void Increments()
{
	for (int i = 0; i < 10; ++i)
	{
		g_counter.increment();
		std::cout << g_counter.get() << std::endl;
	}
}

void Decrements()
{
	for (int i = 0; i < 5; ++i)
	{
		g_counter.decrement();
		std::cout << g_counter.get() << std::endl;
	}
}

int main(void)
{
	std::thread t1(Increments);
	std::thread t2(Decrements);

	t1.join();
	t2.join();

	system("pause");
	return 0;
}