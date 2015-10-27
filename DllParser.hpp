#pragma once
#include <Windows.h>
#include <string>
#include <map>
#include <functional>
using namespace std;

class DllParser
{
public:

	DllParser():m_hMod(nullptr)
	{
	}

	~DllParser()
	{
		UnLoad();
	}

	bool Load(const string& dllPath)
	{
		m_hMod = LoadLibraryA(dllPath.data());
		if (nullptr == m_hMod)
		{
			printf("LoadLibrary failed\n");
			return false;
		}

		return true;
	}

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
	std::function<T> GetFunction(const string& funcName)
	{
		auto it = m_map.find(funcName);
		if (it == m_map.end())
		{
			auto addr = GetProcAddress(m_hMod, funcName.c_str());
			if (!addr)
				return nullptr;
			m_map.insert(std::make_pair(funcName, addr));
			it = m_map.find(funcName);
		}
		
		return std::function<T>((T*) (it->second));
	}

	template <typename T, typename... Args>
	typename std::result_of<std::function<T>(Args...)>::type ExcecuteFunc(const string& funcName, Args&&... args)
	{
		auto f = GetFunction<T>(funcName);
		if (f == nullptr)
		{
			string s = "can not find this function " + funcName;
			throw std::exception(s.c_str());
		}			

		return f(std::forward<Args>(args)...);
	}

private:
	HMODULE m_hMod;
	std::map<string, FARPROC> m_map;
};
