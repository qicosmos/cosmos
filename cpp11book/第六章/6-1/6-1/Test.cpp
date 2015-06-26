#define _CRT_SECURE_NO_WARNINGS

#include "Timer.hpp"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>

void fun()
{
	std::cout << "hello world" << std::endl;
}

void Test()
{
	std::cout << "\nTest()\n";

	Timer t; //开始计时
	fun();

	std::cout << t.elapsed_seconds() << std::endl; //打印fun函数耗时多少秒
	std::cout << t.elapsed_nano() << std::endl; //打印纳秒
	std::cout << t.elapsed_micro() << std::endl; //打印微秒
	std::cout << t.elapsed() << std::endl; //打印毫秒
	std::cout << t.elapsed_seconds() << std::endl; //打印秒
	std::cout << t.elapsed_minutes() << std::endl; //打印分钟
	std::cout << t.elapsed_hours() << std::endl; //打印小时
}


int main(void)
{
	Test();

	system("pause");
	return 0;
}