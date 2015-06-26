#include <vector>
#include <map>
#include <string>
#include <iostream>

using namespace std;

struct Complicated
{
	int year;
	double country;
	string name;

	Complicated(int a, double b, string c) : year(a), country(b), name(c)
	{
		cout << "is constructed" << endl;
	}

	Complicated(const Complicated& other) : year(other.year),
		country(other.country), name(other.name)
	{
		cout << "is moved" << endl;
	}
};

int main(void)
{
	map<int, Complicated> m;
	int anInt = 4;
	double aDouble = 5.0;
	string aString = "C++";

	cout << "--insert--" << endl;
	m.insert(make_pair(4, Complicated(anInt, aDouble, aString)));

	cout << "--emplace--" << endl;
	m.emplace(4, Complicated(anInt, aDouble, aString));

	cout << "--emplace_back--" << endl;
	vector<Complicated> v;
	v.emplace_back(anInt, aDouble, aString);

	cout << "--push_back--" << endl;
	v.push_back(Complicated(anInt, aDouble, aString));

	system("pause");
	return 0;
}