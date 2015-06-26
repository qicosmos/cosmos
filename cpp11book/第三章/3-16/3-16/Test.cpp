#include "ScopeGuard.hpp"

#include <iostream>
#include <functional>

void TestScopeGuard()
{
	std::function < void()> f = []()
	{ std::cout << "cleanup from unnormal exit" << std::endl; };
	//正常退出
	{
		auto gd = MakeGuard(f);
		//...
		gd.Dismiss();  //表明前面我是正常的清理了资源，属于正常退出的
	}

	//异常退出
	try
	{
		auto gd = MakeGuard(f);
		//...
		throw 1;
	}
	catch (...)
	{
		std::cout << "捕获到了一个异常！！\n\n";
	}

	//非正常退出
	{
		auto gd = MakeGuard(f);
		return;  //非正常退出表示资源还没清理呢，，等着ScopeGuard自动清理
		//...
	}
}


int main(void)
{
	TestScopeGuard();

	system("pause");
	return 0;
}
