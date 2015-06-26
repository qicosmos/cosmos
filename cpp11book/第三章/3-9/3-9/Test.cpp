#include "MyDLL.h"

#include <iostream>
#include <windows.h>

void TestDll()
{
	typedef int(*pMax)(int a, int b);
	typedef int(*pGet)(int a);

	HINSTANCE hDLL = LoadLibrary("MyDll.dll");
	if (hDLL == nullptr)
	{
		return;
	}

	pMax Max = (pMax)GetProcAddress(hDLL, "Max");
	if (Max == nullptr)
	{
		return;
	}

	int ret = Max(5, 8);

	pGet Get = (pGet)GetProcAddress(hDLL, "Get");
	if (Get == nullptr)
	{
		return;
	}

	int ret2 = Get(5);

	FreeLibrary(hDLL);
}

int main(void)
{
	TestDll();

	system("pause");
	return 0;
}