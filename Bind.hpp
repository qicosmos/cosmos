#pragma once
#include <tuple>
#include <type_traits>
using namespace std;

template<int...>
struct IndexTuple{};

template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N - 1, N - 1, Indexes...>{};

template<int... indexes>
struct MakeIndexes<0, indexes...>
{
    typedef IndexTuple<indexes...> type;
};

template <int I>
struct Placeholder
{
};

Placeholder<1> _1; Placeholder<2> _2; Placeholder<3> _3; Placeholder<4> _4; Placeholder<5> _5; 
Placeholder<6> _6; Placeholder<7> _7;Placeholder<8> _8; Placeholder<9> _9; Placeholder<10> _10;

// result type traits
template <typename F>
struct result_traits : result_traits<decltype(&F::operator())> {};

template <typename T>
struct result_traits<T*> : result_traits<T> {};

/* check function */

template <typename R, typename... P>
struct result_traits<R(*)(P...)> { typedef R type; };

/* check member function */
template <typename R, typename C, typename... P> 
struct result_traits<R(C::*)(P...)> { typedef R type; };

template <typename T, class Tuple>
inline auto select(T&& val, Tuple&)->T&&
{
    return std::forward<T>(val);
}

template <int I, class Tuple>
inline auto select(Placeholder<I>&, Tuple& tp) -> decltype(std::get<I - 1>(tp))
{
    return std::get<I - 1>(tp);
}

// The invoker for call a callable
template <typename T>
struct is_pointer_noref
    : std::is_pointer<typename std::remove_reference<T>::type>
{};

template <typename T>
struct is_memfunc_noref
    : std::is_member_function_pointer<typename std::remove_reference<T>::type>
{};

template <typename R, typename F, typename... P>
inline typename std::enable_if<is_pointer_noref<F>::value,
R>::type invoke(F&& f, P&&... par)
{
    return (*std::forward<F>(f))(std::forward<P>(par)...);
}

template <typename R, typename F, typename P1, typename... P>
inline typename std::enable_if<is_memfunc_noref<F>::value && is_pointer_noref<P1>::value,
R>::type invoke(F&& f, P1&& this_ptr, P&&... par)
{
    return (std::forward<P1>(this_ptr)->*std::forward<F>(f))(std::forward<P>(par)...);
}

template <typename R, typename F, typename P1, typename... P>
inline typename std::enable_if<is_memfunc_noref<F>::value && !is_pointer_noref<P1>::value,
R>::type invoke(F&& f, P1&& this_obj, P&&... par)
{
    return (std::forward<P1>(this_obj).*std::forward<F>(f))(std::forward<P>(par)...);
}

template <typename R, typename F, typename... P>
inline typename std::enable_if<!is_pointer_noref<F>::value && !is_memfunc_noref<F>::value,
R>::type invoke(F&& f, P&&... par)
{
    return std::forward<F>(f)(std::forward<P>(par)...);
}

template<typename Fun, typename... Args>
struct Bind_t
{
    typedef typename decay<Fun>::type FunType;
    typedef std::tuple<typename decay<Args>::type...> ArgType;

    typedef typename result_traits<FunType>::type     result_type;
public:
    template<class F, class... BArgs>
    Bind_t(F& f,  BArgs&... args) : m_func(f), m_args(args...)
    {
    }

    template<typename F, typename... BArgs>
    Bind_t(F&& f, BArgs&&... par) : m_func(std::move(f)), m_args(std::move(par)...)
    {}

    template <typename... CArgs>
    result_type operator()(CArgs&&... args)
    {
        return do_call(MakeIndexes<std::tuple_size<ArgType>::value>::type(), 
std::forward_as_tuple(std::forward<CArgs>(args)...));
    }

    template<typename ArgTuple, int... Indexes >
    result_type do_call(IndexTuple< Indexes... >& in, ArgTuple& argtp)
    {
        return simple::invoke<result_type>(m_func, select(std::get<Indexes>(m_args), 
argtp)...);
        //return m_func(select(std::get<Indexes>(m_args), argtp)...);
    }

private:
    FunType m_func;
    ArgType m_args;
};

template <typename F, typename... P>
inline Bind_t<F, P...> Bind(F&& f, P&&... par)
{
    return Bind_t<F, P...>(std::forward<F>(f), std::forward<P>(par)...);
}

template <typename F, typename... P>
inline Bind_t<F, P...> Bind(F& f, P&... par)
{
    return Bind_t<F, P...>(f, par...);
}
