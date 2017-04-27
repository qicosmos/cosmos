#pragma once
#include<string>
#include<unordered_map>
#include<memory>
#include<functional>
using namespace std;
#include<Any.hpp>
#include <NonCopyable.hpp>

class IocContainer : NonCopyable
{
public:
	IocContainer(void){}
	~IocContainer(void){}

	template<class T, typename Depend, typename... Args>
	void RegisterType(const string& strKey)
	{
		std::function<T* (Args...)> function = [](Args... args){ return new T(new Depend(args...)); };//通过闭包擦除了参数类型
		RegisterType(strKey, function);
	}

	template<class T, typename... Args>
	T* Resolve(const string& strKey, Args... args)
	{
		if (m_creatorMap.find(strKey) == m_creatorMap.end())
			returnnullptr;

		Any resolver = m_creatorMap[strKey];
		std::function<T* (Args...)> function = resolver.AnyCast<std::function<T* (Args...)>>();

		return function(args...);
	}

	template<class T, typename... Args>
	std::shared_ptr<T> ResolveShared(const string& strKey, Args... args)
	{
		T* t = Resolve<T>(strKey, args...);

		return std::shared_ptr<T>(t);
	}

private:
	void RegisterType(const string& strKey, Any constructor)
	{
		if (m_creatorMap.find(strKey) != m_creatorMap.end())
			throw std::invalid_argument("this key has already exist!");

		//通过Any擦除了不同类型的构造器
		m_creatorMap.emplace(strKey, constructor);
	}

private:
	unordered_map<string, Any> m_creatorMap;
};
/*test code
struct Base
{
	virtual void Func(){}
	virtual ~Base(){}
};

struct DerivedB : public Base
{
	DerivedB(int a, double b):m_a(a),m_b(b)
	{
	}
	void Func()override
	{
		cout<<m_a+m_b<<endl;
	}
private:
	int m_a;
	double m_b;
};

struct DerivedC : public Base
{
};

struct A
{
	A(Base * ptr) :m_ptr(ptr)
	{		
	}

	void Func()
	{
		m_ptr->Func();
	}
	
	~A()
	{
		if(m_ptr!=nullptr)
		{
			delete m_ptr;
			m_ptr = nullptr;
		}
	}

private:
Base * m_ptr;
};

void TestIoc()
{
	IocContainer ioc;
	ioc.RegisterType<A, DerivedC>(“C”);      //配置依赖关系
	auto c = ioc.ResolveShared<A>(“C”);

	ioc.RegisterType<A, DerivedB, int, double>(“C”);   //注册时要注意DerivedB的参数int和double
	auto b = ioc.ResolveShared<A>(“C”, 1, 2.0); //还要传入参数
	b->Func();
}

*/
