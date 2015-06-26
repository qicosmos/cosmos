#include <iostream>

template <typename T>
void Print(T t)
{
	std::cout << t << std::endl;
}

template <typename T1, typename T2>
void Print(T1 t1, T2 t2)
{
	std::cout << t1 << t2 << std::endl;
}

template <typename T1, typename T2, typename T3>
void Print(T1 t1, T2 t2, T3 t3)
{
	std::cout << t1 << t2 << t3 << std::endl;
}

template <typename T1, typename T2, typename T3, typename T4>
void Print(T1 t1, T2 t2, T3 t3, T4 t4)
{
	std::cout << t1 << t2 << t3 << t4 << std::endl;
}

template <typename T1, typename T2, typename T3, typename T4, typename T5>
void Print(T1 t1, T2 t2, T3 t3, T4 t4, T5 t5)
{
	std::cout << t1 << t2 << t3 << t4 << t5 << std::endl;
}

int main(void)
{
	Print(1);
	Print(1, 2);
	Print(1, 2, 3);
	Print(1, 2, 3, 4);
	Print(1, 2, 3, 4, 5);


	system("pause");
	return 0;
}