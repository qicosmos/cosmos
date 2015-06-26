#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex g_lock;

void func()
{
	g_lock.lock();

	std::cout << "entered thread " << std::this_thread::get_id() << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(1));
	std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;

	g_lock.unlock();
}

int main(void)
{
	std::thread t1(func);
	std::thread t2(func);
	std::thread t3(func);

	t1.join();
	t2.join();
	t3.join();

	system("pause");
	return 0;
}