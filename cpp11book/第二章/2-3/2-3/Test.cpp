#include <iostream>
#include <vector>

class MyString
{
private:
	char* m_data;
	size_t m_len;
	void copy_data(const char* s)
	{
		m_data = new char[m_len + 1];
		memcpy(m_data, s, m_len);
		m_data[m_len] = '\0';
	}

public:
	MyString()
	{
		m_data = NULL;
		m_len = 0;
	}

	MyString(const char* p)
	{
		m_len = strlen(p);
		copy_data(p);
	}

	MyString(const MyString& str)
	{
		m_len = str.m_len;
		copy_data(str.m_data);
		std::cout << "Copy Constructor is called! source:" << str.m_data << std::endl;
	}

	MyString& operator=(const MyString& str)
	{
		if (this != &str)
		{
			m_len = str.m_len;
			copy_data(str.m_data);
		}
		std::cout << "Copy Assignment is called! source:" << str.m_data << std::endl;
		return *this;
	}

	MyString(MyString&& str)
	{
		std::cout << "Move Constructor is called! source:" << str.m_data << std::endl;
		m_len = str.m_len;
		m_data = str.m_data;
		str.m_len = 0;
		str.m_data = NULL;
	}

	MyString& operator=(MyString&& str)
	{
		std::cout << "Move Assignment is called! source:" << str.m_data << std::endl;

		if (this != &str)
		{
			m_len = str.m_len;
			m_data = str.m_data;
			str.m_len = 0;
			str.m_data = NULL;
		}
		
		return *this;
	}

	virtual ~MyString()
	{
		if (m_data != NULL)
		{
			delete[] m_data;
		}
	}
};

int main(void)
{
	MyString a;
	a = MyString("Hello");

	std::vector<MyString> vec;
	vec.push_back(MyString("World"));

	system("pause");
	return 0;
}