#include <cstddef>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>

template<typename T, T mPtr>
struct MemberBinding
{
    constexpr static T value = mPtr;
};

template<typename... Args>
struct Pack
{
    constexpr static const std::size_t value = sizeof...(Args);
};

template<typename...> using void_t = void;

template<typename> struct Members {};

struct SubAg
{
    double value;
};

template<>
struct Members<SubAg>
{
    using type = Pack<MemberBinding<decltype(&SubAg::value), &SubAg::value>>;
    constexpr  static const size_t value = type::value;
    constexpr static const char *name = "SubAg";
    static const char *const *names()
    {
        static const char *rv[] = { "value" };
        return rv;
    }
};

struct Aggregate
{
    int member1;
    SubAg sa;
    std::string member2;
};

template<>
struct Members<Aggregate>
{
    using type = Pack<
            MemberBinding<decltype(&Aggregate::member1), &Aggregate::member1>,
            MemberBinding<decltype(&Aggregate::sa), &Aggregate::sa>,
            MemberBinding<decltype(&Aggregate::member2), &Aggregate::member2>
    >;
    constexpr static const size_t value = type::value;
    constexpr static const char *name = "Aggregate";
    static const char *const *names()
    {
        static const char *rv[] = { "member1", "sa", "member2" };
        return rv;
    }
};

/// Overload for T where there is a Members<T> specialization
template<typename T>
void to_str_impl(std::ostream &out, const T &v, void_t<typename Members<T>::type> *);

/// Less specific overload
template<typename T>
void to_str_impl(std::ostream &out, const T &v, ...)
{
    out << v;
}

/// General case
template<typename A, typename MT, MT MPTR, typename... Rest,std::size_t I0, std::size_t... I>
void printPack(std::ostream &out, const A &v, Pack<MemberBinding<MT, MPTR>, Rest...> *, std::index_sequence<I0, I...>)
{
    using M = Members<A>;
    out << ' ' << I0 << ':' << M::names()[I0] << ':';
    to_str_impl(out, v.*MPTR, (void *)nullptr);
    printPack(out, v, (Pack<Rest...> *)nullptr, std::index_sequence<I...>{});
}

template<typename A>
void printPack(std::ostream &out, const A &v, Pack<> *,std::index_sequence<>)
{
}

template<typename T>
void to_str_impl(std::ostream &out, const T &v, void_t<typename Members<T>::type> *)
{
    using M = Members<T>;
    out << '(' << M::name;
    printPack(out, v, (typename M::type *)nullptr, std::make_index_sequence<Members<T>::value>{});
    out << ')';
}

template<typename T>
std::string to_str(const T &v)
{
    std::ostringstream prv;
    to_str_impl(prv, v, (void *)nullptr);
    return prv.str();
}

std::string fun(Aggregate &a) { return to_str(a); }

//get field by index, name;get field name and method name; invoke by method name;
template <size_t I, typename T>
decltype(auto) get(const T &v)
{

}

int main(int argc, const char *argv[])
{
    Aggregate a{ 5, { 3.14 }, "Bellevue" };
    std::cout << to_str(a) << std::endl;
    return 0;
}
