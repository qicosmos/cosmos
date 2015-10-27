#pragma once
#define HAS_MEMBER(member)\
template<typename T, typename... Args>struct has_member_##member\
{\
private:\
		template<typename U> static auto Check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type()); \
	template<typename U> static std::false_type Check(...);\
public:\
	enum{value = std::is_same<decltype(Check<T>(0)), std::true_type>::value};\
};\

HAS_MEMBER(Foo)
HAS_MEMBER(Before)
HAS_MEMBER(After)

#include <NonCopyable.hpp>
template<typename Func, typename... Args>
struct Aspect : NonCopyable
{
	Aspect(Func&& f) : m_func(std::forward<Func>(f))
	{
	}

	template<typename T>
	typename std::enable_if<has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
	{
		aspect.Before(std::forward<Args>(args)...);//核心逻辑之前的切面逻辑
		m_func(std::forward<Args>(args)...);//核心逻辑
		aspect.After(std::forward<Args>(args)...);//核心逻辑之后的切面逻辑
	}

	template<typename T>
	typename std::enable_if<has_member_Before<T, Args...>::value&&!has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
	{
		aspect.Before(std::forward<Args>(args)...);//核心逻辑之前的切面逻辑
		m_func(std::forward<Args>(args)...);//核心逻辑
	}

	template<typename T>
	typename std::enable_if<!has_member_Before<T, Args...>::value&&has_member_After<T, Args...>::value>::type Invoke(Args&&... args, T&& aspect)
	{
		m_func(std::forward<Args>(args)...);//核心逻辑
		aspect.After(std::forward<Args>(args)...);//核心逻辑之后的切面逻辑
	}

	template<typename Head, typename... Tail>
	void Invoke(Args&&... args, Head&&headAspect, Tail&&... tailAspect)
	{
		headAspect.Before(std::forward<Args>(args)...);
		Invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
		headAspect.After(std::forward<Args>(args)...);
	}

private:
	Func m_func; //被织入的函数
};
template<typename T> using identity_t = T;

//AOP的辅助函数，简化调用
template<typename... AP, typename... Args, typename Func>
void Invoke(Func&&f, Args&&... args)
{
	Aspect<Func, Args...> asp(std::forward<Func>(f));
	asp.Invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}

/*TEST CODE
struct TimeElapsedAspect
{
	void Before(int i)
	{
		m_lastTime = m_t.elapsed();
	}

	void After(int i)
	{
		cout <<"time elapsed: "<< m_t.elapsed() - m_lastTime << endl;
	}

private:
	double m_lastTime;
	Timer m_t;
};

struct LoggingAspect
{
	void Before(int i)
	{
		std::cout <<"entering"<< std::endl;
	}

	void After(int i)
	{
		std::cout <<"leaving"<< std::endl;
	}
};

void foo(int a)
{
	cout <<"real HT function: "<<a<< endl;
}

int main()
{
	Invoke<LoggingAspect, TimeElapsedAspect>(&foo, 1); //织入方法
cout <<"-----------------------"<< endl;
	Invoke<TimeElapsedAspect, LoggingAspect>(&foo, 1);

	return 0;
}
*/
