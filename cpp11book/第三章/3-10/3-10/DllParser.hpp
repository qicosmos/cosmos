#ifndef _DLLPARSER_HPP_
#define _DLLPARSER_HPP_

#include <Windows.h>
#include <string>
#include <map>
#include <functional>
#include <iostream>


class DllParser
{
public:

	DllParser()
	{
	}

	~DllParser()
	{
		UnLoad();
	}

	bool Load(const std::string& dllPath)
	{
		m_hMod = LoadLibrary(dllPath.data());
		if (nullptr == m_hMod)
		{
			std::cout << "LoadLibrary failed\n";
			return false;
		}

		return true;
	}

	template <typename T, typename... Args>
	typename std::result_of<std::function<T>(Args...)>::type
		ExcecuteFunc(const std::string& funcName, Args&&... args)
	{
		auto f = GetFunction<T>(funcName);

		if (f == nullptr)
		{
			std::string s = "can not find this function " + funcName;
			throw std::exception(s.c_str());
		}            

		return f(std::forward<Args>(args)...);
	}

private:
	bool UnLoad()
	{
		if (m_hMod == nullptr)
			return true;

		auto b = FreeLibrary(m_hMod);
		if (!b)
			return false;

		m_hMod = nullptr;
		return true;
	}

	template <typename T>
	T* GetFunction(const std::string& funcName)
	{
		auto addr = GetProcAddress(m_hMod, funcName.c_str());
		return (T*) (addr);
	}

private:
	HMODULE m_hMod;
	std::map<std::string, FARPROC> m_map;
};

#endif  //_DLLPARSER_HPP_