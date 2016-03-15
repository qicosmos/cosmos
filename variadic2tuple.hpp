
#define MAKE_PAIR(text) std::pair<std::string, decltype(text)>{#text, text}

template<typename T, typename T1, typename... Args>
constexpr static inline auto apply(T const & t, const T1& first, const Args&... args)
{
	return apply(std::tuple_cat(t, std::make_tuple(MAKE_PAIR(first))), args...);
}

template<typename T>
constexpr static inline auto apply(T const & args)
{
	return args;
}

#define META(...) auto meta(){ return apply(std::tuple<>(), __VA_ARGS__); }

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
	auto pp = p.meta();
	std::cout << typeid(pp).name() << std::endl;
*/
