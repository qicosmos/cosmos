#include "SyncQueue.hpp"

#include <thread>
#include <iostream>
#include <mutex>

SyncQueue<int> syncQueue(5);

void PutDatas()
{
	for (int i = 0; i < 20; ++i)
	{
		syncQueue.Put(888);
	}
}

void TakeDatas()
{
	int x = 0;

	for (int i = 0; i < 20; ++i)
	{
		syncQueue.Take(x);
		std::cout << x << std::endl;
	}
}

int main(void)
{
	std::thread t1(PutDatas);
	std::thread t2(TakeDatas);

	t1.join();
	t2.join();

	system("pause");
	return 0;
}