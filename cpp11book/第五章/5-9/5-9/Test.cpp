#include <iostream>
#include <thread>
#include <utility>
#include <future>
#include <vector>

int func(int x) { return x + 2; }

int main(void)
{
	std::packaged_task<int(int)> tsk(func);
	std::future<int> fut = tsk.get_future();  //获取future

	std::thread(std::move(tsk), 2).detach();

	int value = fut.get();  //等待task完成并获取返回值
	std::cout << "The result is " << value << ".\n";

	std::vector<std::shared_future<int>> v;
	std::shared_future<int> f = std::async(std::launch::async, [](int a, int b){return a + b; }, 2, 3);

	v.push_back(f);
	std::cout << "The shared_future result is " << v[0].get() << std::endl;

	system("pause");
	return 0;
}