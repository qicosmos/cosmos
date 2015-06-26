#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::timed_mutex mutex;

void work()
{
	std::chrono::milliseconds timeout(100);

	while (true)
	{
		if (mutex.try_lock_for(timeout))
		{
			std::cout << std::this_thread::get_id() << ": do work with the mutex" << std::endl;

			std::chrono::milliseconds sleepDuration(250);
			std::this_thread::sleep_for(sleepDuration);

			mutex.unlock();
			std::this_thread::sleep_for(sleepDuration);
		}
		else
		{
			std::cout << std::this_thread::get_id() << ": do work without the mutex" << std::endl;

			std::chrono::milliseconds sleepDuration(100);
			std::this_thread::sleep_for(sleepDuration);
		}
	}
}

int main(void)
{
	std::thread t1(work);
	std::thread t2(work);

	t1.join();
	t2.join();

	system("pause");
	return 0;
}