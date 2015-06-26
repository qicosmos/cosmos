#include <boost/variant.hpp>
#include <iostream>
#include <string>

struct VariantVisitor : public boost::static_visitor<void>
{
	void operator()(int a)
	{
		std::cout << "int" << std::endl;
	}
	void operator()(short a)
	{
		std::cout << "short" << std::endl;
	}
	void operator()(double a)
	{
		std::cout << "double" << std::endl;
	}
	void operator()(std::string a)
	{
		std::cout << "std::string" << std::endl;
	}
};


int main(void)
{
	VariantVisitor visitor;
	boost::variant<int, short, double, std::string> v = 1;
	boost::apply_visitor(visitor, v);

	system("pause");
	return 0;
}