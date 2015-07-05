#pragma once
#include <type_traits>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <cstring>
using namespace std;

namespace detail
{
	const char* strue = "true";
	const char* sfalse = "false";

	template <typename To, typename From>
	struct Converter
	{
	};

	//to numeric
	template <typename From>
	struct Converter<int, From>
	{
		static int convert(const string& from)
		{
			return std::atoi(from.c_str());
		}

		static int convert(const char* from)
		{
			return std::atoi(from);
		}
	};

	template <typename From>
	struct Converter<long, From>
	{
		static long convert(const string& from)
		{
			return std::atol(from.c_str());
		}

		static long convert(const char* from)
		{
			return std::atol(from);
		}
	};

	template <typename From>
	struct Converter<long long, From>
	{
		static long long convert(const string& from)
		{
			return std::atoll(from.c_str());
		}

		static long long convert(const char* from)
		{
			return std::atoll(from);
		}
	};

	template <typename From>
	struct Converter<double, From>
	{
		static double convert(const string& from)
		{
			return std::atof(from.c_str());
		}

		static double convert(const char* from)
		{
			return std::atof(from);
		}
	};

	template <typename From>
	struct Converter<float, From>
	{
		static float convert(const string& from)
		{
			return (float)std::atof(from.c_str());
		}

		static float convert(const char* from)
		{
			return (float)std::atof(from);
		}
	};

	//to bool
	template <typename From>
	struct Converter<bool, From>
	{
		static typename std::enable_if<std::is_integral<From>::value, bool>::type convert(From from)
		{
			return !!from;
		}
	};

	bool checkbool(const char* from, const size_t len, const char* s)
	{
		for (size_t i = 0; i < len; i++)
		{
			if (from[i] != s[i])
			{
				return false;
			}
		}

		return true;
	}

	static bool convert(const char* from)
	{
		const unsigned int len = strlen(from);
		if (len != 4 && len != 5)
			throw std::invalid_argument("argument is invalid");

		bool r = true;
		if (len == 4)
		{
			r = checkbool(from, len, strue);

			if (r)
				return true;
		}
		else
		{
			r = checkbool(from, len, sfalse);

			if (r)
				return false;
		}

		throw std::invalid_argument("argument is invalid");
	}

	template <>
	struct Converter<bool, string>
	{
		static bool convert(const string& from)
		{
			return detail::convert(from.c_str());
		}
	};

	template <>
	struct Converter<bool, const char*>
	{
		static bool convert(const char* from)
		{
			return detail::convert(from);
		}
	};

	template <>
	struct Converter<bool, char*>
	{
		static bool convert(char* from)
		{
			return detail::convert(from);
		}
	};

	template <unsigned N>
	struct Converter<bool, const char[N]>
	{
		static bool convert(const char(&from)[N])
		{
			return detail::convert(from);
		}
	};

	template <unsigned N>
	struct Converter<bool, char[N]>
	{
		static bool convert(const char(&from)[N])
		{
			return detail::convert(from);
		}
	};

	//to string
	template <typename From>
	struct Converter<string, From>
	{
		static string convert(const From& from)
		{
			return std::to_string(from);
		}
	};
}

template <typename To, typename From>
typename std::enable_if<!std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
	return detail::Converter<To, From>::convert(from);
}

template <typename To, typename From>
typename std::enable_if<std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
{
	return from;
}

