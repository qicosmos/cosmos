#include <iostream>
#include <string>

int main(void)
{
	double f = 1.53;
	std::string f_str = std::to_string(f);
	std::cout << f_str << std::endl;

	double f1 = 4.125;
	std::wstring f_str1 = std::to_wstring(f1);
	std::wcout << f_str1 << std::endl;

	const char* str1 = "10";
	const char* str2 = "3.1415926";
	const char* str3 = "31337 with words";
	const char* str4 = "words and 2";

	int num1 = std::atoi(str1);
	int num2 = std::atoi(str2);
	int num3 = std::atoi(str3);
	int num4 = std::atoi(str4);
	double f2 = std::atof(str2);

	std::cout << "std::atoi(\"" << str1 << "\") is " << num1 << '\n'; 
	std::cout << "std::atoi(\"" << str2 << "\") is " << num2 << '\n'; 
	std::cout << "std::atoi(\"" << str3 << "\") is " << num3 << '\n'; 
	std::cout << "std::atoi(\"" << str4 << "\") is " << num4 << '\n'; 
	std::cout << "std::atoi(\"" << str2 << "\") is " << f2 << std::endl; 

	system("pause");
	return 0;
}