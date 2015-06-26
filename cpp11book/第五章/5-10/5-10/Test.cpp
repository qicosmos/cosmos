#include <iostream>
#include <future>

void TestAsync()
{
	std::future<int> f1 = std::async(std::launch::async, [](){
		return 8;
	});

	std::cout << f1.get() << std::endl; //output: 8

	std::future<void> f2 = std::async(std::launch::async, [](){
		std::cout << 8 << std::endl; return;
	});

	f2.wait(); //output: 8

	std::future<int> future = std::async(std::launch::async, [](){
		std::this_thread::sleep_for(std::chrono::seconds(3));
		return 8;
	});

	std::cout << "waiting...\n";
	std::future_status status;

	do {
		status = future.wait_for(std::chrono::seconds(1));

		if (status == std::future_status::deferred)
		{
			std::cout << "deferred\n";
		}
		else if (status == std::future_status::timeout)
		{
			std::cout << "timeout\n";
		}
		else if (status == std::future_status::ready)
		{
			std::cout << "ready!\n";
		}
	} while (status != std::future_status::ready);

	std::cout << "result is " << future.get() << '\n';
}

int main(void)
{
	TestAsync();

	system("pause");
	return 0;
}