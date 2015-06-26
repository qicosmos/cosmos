#include <iostream>
#include <functional>
#include <type_traits>

template<typename T>
class Task;

template<typename R, typename...Args>
class Task<R(Args...)>
{
public:
	Task(std::function<R(Args...)>&& f) : m_fn(std::move(f)){}

	Task(std::function<R(Args...)>& f) : m_fn(f){}

	template<typename... Args>
	R Run(Args&&... args)
	{
		return m_fn(std::forward<Args>(args)...);
	}

	template<typename F>
	auto Then(F& f)->Task<typename std::result_of<F(R)>::type(Args...)>
	{
		using return_type = typename std::result_of<F(R)>::type;

		auto func = std::move(m_fn);

		return Task<return_type(Args...)>([func, f](Args&&... args)
		{return f(func(std::forward<Args>(args)...)); });
	}

private:
	std::function<R(Args...)> m_fn;
};


void TestTask()
{
	Task<int(int)> task = [](int i){ return i; };
	auto result = task.Then([](int i){return i + 1; }).Then([](int i){
		return i + 2; }).Then([](int i){return i + 3; }).Run(1);
	std::cout << result << std::endl;  // 7
}


int main(void)
{
	TestTask();

	system("pause");
	return 0;
}