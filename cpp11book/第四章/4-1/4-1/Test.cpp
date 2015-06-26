#include <memory>
#include <type_traits>
#include <iostream>

//支持普通指针
template <class T, class... Args> inline
typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args&&...args)
{
	return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

//支持动态数组
template <class T> inline
typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0, std::unique_ptr<T>>::type
make_unique(size_t size)
{
	typedef typename std::remove_extent<T>::type U;
	return std::unique_ptr<T>(new U[size]());
}

//过滤掉定长数组的情况
template <class T, class... Args>
typename std::enable_if<std::extent<T>::value != 0, void>::type
make_unique(Args&&...) = delete;

int main(void)
{
	auto unique = make_unique<int>(100);
	std::cout << *unique << std::endl;

	auto unique2 = make_unique<int[]>(10);

	for (int i = 0; i < 10; i++)
	{
		unique2[i] = i;
	}
	for (int i = 0; i < 10; i++)
	{
		std::cout << unique2[i] << " ";
	}
	std::cout << std::endl;

	system("pause");
	return 0;
}