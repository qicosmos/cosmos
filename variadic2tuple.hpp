
#define MAKE_PAIR(text) std::pair<std::string, decltype(text)>{#text, text}

template<typename T>
constexpr static inline auto apply(T const & args)
{
	return args;
}

template<typename T, typename T1, typename... Args>
constexpr static inline auto apply(T const & t, const T1& first, const Args&... args)
{
	return apply(std::tuple_cat(t, std::make_tuple(MAKE_PAIR(first))), args...);
}

#define META1(...) auto meta1(){ return apply(std::tuple<>(), __VA_ARGS__); }

/*
struct person
{
	int age;
	std::string name;
	int a;
	double b;
	META(age, name, a, b);
};
  person p{ 20, "tom" };
	auto pp = p.meta1();
	std::cout << typeid(pp).name() << std::endl;
*/

/**************update*****************/
template<unsigned N>
std::array<std::string, N> split(const std::string& s, const char delimiter)
{
	size_t start = 0;
	size_t end = s.find_first_of(delimiter);

	std::array<std::string, N> output;

	size_t i = 0;
	while (end <= std::string::npos)
	{
		output[i++] = std::move(s.substr(start, end - start));
		if (end == std::string::npos)
			break;

		start = end + 2;
		end = s.find_first_of(delimiter, start);
	}

	return output;
}

template<unsigned N, typename T>
constexpr static inline auto make(const std::array<std::string, N>&ar, unsigned index, T& args)
{
	return args;
}

template<unsigned N, typename T, typename T1, typename... Args>
constexpr static inline auto make(const std::array<std::string, N>&ar, unsigned index, T const & t, T1& first, Args&... args)
{
	return make(ar, index + 1, std::tuple_cat(t, std::make_tuple(std::pair<std::string, T1&>(ar[index], first))), args...);
}

#define VA_ARGS_NUM(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value

#define META(...) auto meta(){\
	auto ar = split<VA_ARGS_NUM(__VA_ARGS__)>(#__VA_ARGS__, ',');\
	return make(ar, 0, std::tuple<>(), __VA_ARGS__);\
}

//split literal string into an array at runtime, then use the key and the value to make a tuple<pair<string, somevalue>,...>
