#include <iostream>
#include <string>
#include <codecvt>

int main(void)
{
	std::wstring str = L"我是中国人yzb";
	std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> 
		converter(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));

	std::string narrowStr = converter.to_bytes(str);

	std::wstring wstr = converter.from_bytes(narrowStr);
	std::cout << narrowStr << std::endl;

	std::wcout.imbue(std::locale("chs"));  //初始化cout为中文输出
	std::wcout << wstr << std::endl;

	system("pause");
	return 0;
}