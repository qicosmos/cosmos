#include <iostream>

int main(void)
{
	int a = 1.1;  
	int b = { 1.1 };  //error

	float fa = 1e40;
	float fb = { 1e40 };  //error

	float fc = (unsigned long long)-1;
	float fd = { (unsigned long long)-1 };  //error
	float fe = (unsigned long long)1;
	float ff = { (unsigned long long)1 };  //OK

	const int x = 1024;
	const int y = 1;

	char c = x;
	char d = { x };  //error
	char e = y;
	char f = { y };

	system("pause");
	return 0;
}