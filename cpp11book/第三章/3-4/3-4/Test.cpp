#include <iostream>

template <typename T>
T* Instance()
{
	return new T();
}

template <typename T, typename T0>
T* Instance(T0 arg0)
{
	return new T(arg0);
}


template <typename T, typename T0, typename T1>
T* Instance(T0 arg0, T1 arg1)
{
	return new T(arg0, arg1);
}

template <typename T, typename T0, typename T1, typename T2>
T* Instance(T0 arg0, T1 arg1, T2 arg2)
{
	return new T(arg0, arg1, arg2);
}

template <typename T, typename T0, typename T1, typename T2, typename T3>
T* Instance(T0 arg0, T1 arg1, T2 arg2, T3 arg3)
{
	return new T(arg0, arg1, arg2, arg3);
}

template <typename T, typename T0, typename T1, typename T2, typename T3, typename T4>
T* Instance(T0 arg0, T1 arg1, T2 arg2, T3 arg3, T4 arg4)
{
	return new T(arg0, arg1, arg2, arg3, arg4);
}

struct A
{
	A(int){}
};

struct B
{
	B(int, double){}
};

int main(void)
{
	A* pa = Instance<A>(1);
	B* pb = Instance<B>(1, 2);

	system("pause");
	return 0;
}