#include <iostream>
#include <vector>

template <class ContainerT>
class Foo
{
	typename ContainerT::iterator it_;  //类型定义可能有问题
	//若想在 const ContainerT 下通过编译请使用下面it_定义：
	//decltype(std::declval<ContainerT>().begin()) it_;
public:
	void func(ContainerT& container)
	{
		it_ = container.begin();
	}

	//...
};

int main(void)
{
	typedef const std::vector<int> container_t;  //这里有const属性，编译会报一大堆错误信息
	container_t arr;

	Foo<container_t> foo;
	foo.func(arr);

	system("pause");
	return 0;
}