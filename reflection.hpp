#include <iostream>
#include <sstream>
#include <tuple>

/******************************************/
/* arg list expand macro, now support 40 args */
#define MAKE_ARG_LIST_1(op, arg, ...)   op(arg)
#define MAKE_ARG_LIST_2(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_1(op, __VA_ARGS__))
#define MAKE_ARG_LIST_3(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_2(op, __VA_ARGS__))
#define MAKE_ARG_LIST_4(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_3(op, __VA_ARGS__))
#define MAKE_ARG_LIST_5(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_4(op, __VA_ARGS__))
#define MAKE_ARG_LIST_6(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_5(op, __VA_ARGS__))
#define MAKE_ARG_LIST_7(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_6(op, __VA_ARGS__))
#define MAKE_ARG_LIST_8(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_7(op, __VA_ARGS__))
#define MAKE_ARG_LIST_9(op, arg, ...)   op(arg), MARCO_EXPAND(MAKE_ARG_LIST_8(op, __VA_ARGS__))
#define MAKE_ARG_LIST_10(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_9(op, __VA_ARGS__))
#define MAKE_ARG_LIST_11(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_10(op, __VA_ARGS__))
#define MAKE_ARG_LIST_12(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_11(op, __VA_ARGS__))
#define MAKE_ARG_LIST_13(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_12(op, __VA_ARGS__))
#define MAKE_ARG_LIST_14(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_13(op, __VA_ARGS__))
#define MAKE_ARG_LIST_15(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_14(op, __VA_ARGS__))
#define MAKE_ARG_LIST_16(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_15(op, __VA_ARGS__))
#define MAKE_ARG_LIST_17(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_16(op, __VA_ARGS__))
#define MAKE_ARG_LIST_18(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_17(op, __VA_ARGS__))
#define MAKE_ARG_LIST_19(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_18(op, __VA_ARGS__))
#define MAKE_ARG_LIST_20(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_19(op, __VA_ARGS__))
#define MAKE_ARG_LIST_21(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_20(op, __VA_ARGS__))
#define MAKE_ARG_LIST_22(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_21(op, __VA_ARGS__))
#define MAKE_ARG_LIST_23(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_22(op, __VA_ARGS__))
#define MAKE_ARG_LIST_24(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_23(op, __VA_ARGS__))
#define MAKE_ARG_LIST_25(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_24(op, __VA_ARGS__))
#define MAKE_ARG_LIST_26(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_25(op, __VA_ARGS__))
#define MAKE_ARG_LIST_27(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_26(op, __VA_ARGS__))
#define MAKE_ARG_LIST_28(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_27(op, __VA_ARGS__))
#define MAKE_ARG_LIST_29(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_28(op, __VA_ARGS__))
#define MAKE_ARG_LIST_30(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_29(op, __VA_ARGS__))
#define MAKE_ARG_LIST_31(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_30(op, __VA_ARGS__))
#define MAKE_ARG_LIST_32(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_31(op, __VA_ARGS__))
#define MAKE_ARG_LIST_33(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_32(op, __VA_ARGS__))
#define MAKE_ARG_LIST_34(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_33(op, __VA_ARGS__))
#define MAKE_ARG_LIST_35(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_34(op, __VA_ARGS__))
#define MAKE_ARG_LIST_36(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_35(op, __VA_ARGS__))
#define MAKE_ARG_LIST_37(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_36(op, __VA_ARGS__))
#define MAKE_ARG_LIST_38(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_37(op, __VA_ARGS__))
#define MAKE_ARG_LIST_39(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_38(op, __VA_ARGS__))
#define MAKE_ARG_LIST_40(op, arg, ...)  op(arg), MARCO_EXPAND(MAKE_ARG_LIST_39(op, __VA_ARGS__))

#define RSEQ_N() \
		 119,118,117,116,115,114,113,112,111,110,\
		 109,108,107,106,105,104,103,102,101,100,\
		 99,98,97,96,95,94,93,92,91,90, \
		 89,88,87,86,85,84,83,82,81,80, \
		 79,78,77,76,75,74,73,72,71,70, \
		 69,68,67,66,65,64,63,62,61,60, \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0

#define ARG_N( \
         _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
		 _61,_62,_63,_64,_65,_66,_67,_68,_69,_70, \
		 _71,_72,_73,_74,_75,_76,_77,_78,_79,_80, \
		 _81,_82,_83,_84,_85,_86,_87,_88,_89,_90, \
		 _91,_92,_93,_94,_95,_96,_97,_98,_99,_100, \
		 _101,_102,_103,_104,_105,_106,_107,_108,_109,_110, \
         _111,_112,_113,_114,_115,_116,_117,_118,_119,N, ...) N

#define MARCO_EXPAND(...)                 __VA_ARGS__
#define APPLY_VARIADIC_MACRO(macro, ...)  MARCO_EXPAND(macro(__VA_ARGS__))

#define ADD_REFERENCE(t)        std::reference_wrapper<decltype(t)>(t)
#define ADD_REFERENCE_CONST(t)  std::reference_wrapper<std::add_const_t<decltype(t)>>(t)
#define OBJECT(t)          t//std::make_pair(#t, ADD_REFERENCE(t))
//#define OBJECT_CONST(t)    std::make_pair(#t, ADD_REFERENCE_CONST(t))
#define MAKE_NAMES(...) #__VA_ARGS__,

//#define SEPERATOR ,
//#define CON_STR_1(element1, ...) #element1
//#define CON_STR_2(element1, ...) #element1 SEPERATOR CON_STR_1(__VA_ARGS__)
//#define CON_STR_3(element1, ...) #element1 SEPERATOR CON_STR_2(__VA_ARGS__)
//#define CON_STR_4(element1, ...) #element1 SEPERATOR CON_STR_3(__VA_ARGS__)
//#define CON_STR_5(element1, ...) #element1 SEPERATOR CON_STR_4(__VA_ARGS__)
//#define MAKE_STR_LIST(...) \
//    MACRO_CONCAT(CON_STR, GET_ARG_COUNT(__VA_ARGS__))(__VA_ARGS__)
//#define get_name(STRUCT_NAME, I) I<arr_##STRUCT_NAME.size()?arr_##STRUCT_NAME[I]:"";

//#define MAKE_TUPLE_CONST(...)   auto tuple() const { return std::make_tuple(__VA_ARGS__); }

//note use MACRO_CONCAT like A##_##B direct may cause marco expand error
#define MACRO_CONCAT(A, B) MACRO_CONCAT1(A, B)
#define MACRO_CONCAT1(A, B) A##_##B

#define MAKE_ARG_LIST(N, op, arg, ...) \
        MACRO_CONCAT(MAKE_ARG_LIST, N)(op, arg, __VA_ARGS__)

#define GET_ARG_COUNT_INNER(...)    MARCO_EXPAND(ARG_N(__VA_ARGS__))
#define GET_ARG_COUNT(...)          GET_ARG_COUNT_INNER(__VA_ARGS__, RSEQ_N())

template<typename...> using void_t = void;

template<typename> struct Members {};

#define MAKE_TUPLE(STRUCT_NAME, ...)\
template<>struct Members<STRUCT_NAME>{\
    constexpr decltype(auto) static apply(){\
        return std::make_tuple(__VA_ARGS__);\
    }\
    using type = void;\
    constexpr static const char *name = #STRUCT_NAME;\
    constexpr  static const size_t value = GET_ARG_COUNT(__VA_ARGS__);\
    constexpr static std::array<const char*, value> arr={MAKE_NAMES(__VA_ARGS__)};\
};

#define EMMBED_TUPLE(STRUCT_NAME, N, ...) \
MAKE_TUPLE(STRUCT_NAME, MAKE_ARG_LIST(N, &STRUCT_NAME::OBJECT, __VA_ARGS__))
//MAKE_TUPLE_CONST(MAKE_ARG_LIST(N, OBJECT, __VA_ARGS__))

#define REFLECTION(STRUCT_NAME, ...) EMMBED_TUPLE(STRUCT_NAME, GET_ARG_COUNT(__VA_ARGS__), __VA_ARGS__)

template<typename F, typename T>
constexpr void for_each_impl(F&&, T&&, void_t<typename Members<std::remove_const_t<std::remove_reference_t<T>>>::type> *);

template<typename F, typename T>
constexpr void for_each_impl(F&& f, T&& t, ...)
{
    std::forward<F>(f)(std::forward<T>(t));
}

template<typename F, typename T, typename... Rest,std::size_t I0, std::size_t... I>
constexpr void apply(F&& f, T&&t, std::tuple<Rest...>&& tp, std::index_sequence<I0, I...>)
{
    for_each_impl(std::forward<F>(f), std::forward<T>(t).*(std::get<I0>(tp)), (void *)nullptr);
    apply(std::forward<F>(f), std::forward<T>(t), (std::tuple<Rest...>&&)tp, std::index_sequence<I...>{});
}

template<typename F, typename T,typename... Rest>
constexpr void apply(F&& f, T&& t, std::tuple<Rest...>&&, std::index_sequence<>)
{
}

template<typename F, typename T>
constexpr void for_each_impl(F&& f, T&&t, void_t<typename Members<std::remove_reference_t<std::remove_const_t<T>>>::type> *)
{
    using M = Members<std::remove_reference_t <std::remove_const_t<T>>>;
    //out << '(' << M::name<<": ";
    apply(std::forward<F>(f), std::forward<T>(t), M::apply(), std::make_index_sequence<M::value>{});
    //out << ')';
}

template<size_t I, typename T>
constexpr decltype(auto) get(T&& t)
{
    using M = Members<std::remove_reference_t <std::remove_const_t<T>>>;
    return std::forward<T>(t).*(std::get<I>(M::apply()));
}

template<typename T, typename F>
constexpr void for_each(T&& t, F&& f)
{
    for_each_impl(std::forward<F>(f), std::forward<T>(t), (void *)nullptr);
};

template<typename T>
std::string to_str(T&& t)
{
    std::ostringstream prv;
    for_each(std::forward<T>(t), [&prv](auto v) { prv<<v; });
    return prv.str();
}

struct city_t{
    int id;
    double num;
};
REFLECTION(city_t, id,num);

struct person_t
{
    int id;
    double num;
    city_t city;
};

REFLECTION(person_t, id,num,city);

int main(int argc, const char *argv[])
{
    city_t city = {20, 3.5};
    person_t p = {1,2.5,city};

    auto rr = get<1>(p);//magic get by index
    for_each(p, [](auto& v){ //magic for_each struct std::forward<T>(t)
        v*=10;
        std::cout<< v <<std::endl;
    });

    return 0;
}
