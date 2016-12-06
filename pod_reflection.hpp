//this code is from magic_get, but more simple and easy to understand the magic.
//modern c++ is really an magic wonderful language!

#include <type_traits>
#include <utility>
#include <iostream>
#include <tuple>    // GCC: reimplement tuple to have non-reverse order of elements

///////////////////// Tuple that holds it's values in the supplied order
namespace sequence_tuple {
    template <std::size_t N, class T>
    struct base_from_member {
        T value;
    };

    template <class I, class ...Tail>
    struct tuple_base;

    template <std::size_t... I, class ...Tail>
    struct tuple_base< std::index_sequence<I...>, Tail... >
            : base_from_member<I , Tail>...
    {
        static constexpr std::size_t size_v = sizeof...(I);

        constexpr tuple_base() noexcept = default;
        constexpr tuple_base(tuple_base&&) noexcept = default;
        constexpr tuple_base(const tuple_base&) noexcept = default;

        constexpr tuple_base(Tail... v) noexcept
                : base_from_member<I, Tail>{ v }...
        {}
    };

    template <>
    struct tuple_base<std::index_sequence<> > {
        static constexpr std::size_t size_v = 0;
    };

    template <std::size_t N, class T>
    constexpr T& get_impl(base_from_member<N, T>& t) noexcept {
        return t.value;
    }

    template <std::size_t N, class T>
    constexpr const T& get_impl(const base_from_member<N, T>& t) noexcept {
        return t.value;
    }

    template <std::size_t N, class T>
    constexpr volatile T& get_impl(volatile base_from_member<N, T>& t) noexcept {
        return t.value;
    }

    template <std::size_t N, class T>
    constexpr const volatile T& get_impl(const volatile base_from_member<N, T>& t) noexcept {
        return t.value;
    }

    template <std::size_t N, class T>
    constexpr T&& get_impl(base_from_member<N, T>&& t) noexcept {
        return std::forward<T>(t.value);
    }

    template <class ...Values>
    struct tuple: tuple_base<
            std::make_index_sequence<sizeof...(Values)>,
            Values...>
    {
        using tuple_base<
                std::make_index_sequence<sizeof...(Values)>,
                Values...
        >::tuple_base;
    };


    template <std::size_t N, class ...T>
    constexpr decltype(auto) get(tuple<T...>& t) noexcept {
        static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
        return get_impl<N>(t);
    }

    template <std::size_t N, class ...T>
    constexpr decltype(auto) get(const tuple<T...>& t) noexcept {
        static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
        return get_impl<N>(t);
    }

    template <std::size_t N, class ...T>
    constexpr decltype(auto) get(const volatile tuple<T...>& t) noexcept {
        static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
        return get_impl<N>(t);
    }

    template <std::size_t N, class ...T>
    constexpr decltype(auto) get(volatile tuple<T...>& t) noexcept {
        static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
        return get_impl<N>(t);
    }

    template <std::size_t N, class ...T>
    constexpr decltype(auto) get(tuple<T...>&& t) noexcept {
        static_assert(N < tuple<T...>::size_v, "Tuple index out of bounds");
        return get_impl<N>(std::move(t));
    }

    template <size_t I, class T>
    using tuple_element = std::remove_reference< decltype(
    sequence_tuple::get<I>( std::declval<T>() )
    ) >;

} // namespace sequence_tuple

namespace detail {

    template <class T, std::size_t N>
    struct array {      // CLANG: misses constexpr on operator[]
        typedef T type;
        T data[N];

        static constexpr std::size_t size() noexcept { return N; }
    };

    template <std::size_t I, class T, std::size_t N>
    constexpr const T& get(const array<T,N>& a) noexcept {
        return a.data[I];
    }

    template <class Array>
    constexpr auto count_nonzeros(Array a) noexcept {
        std::size_t count = 0;
        for (std::size_t i = 0; i < Array::size() && a.data[i]; ++i)
            ++ count;

        return count;
    }

    template <class T> struct identity{};

    constexpr auto id_to_type( std::integral_constant<std::size_t, 20 > ) noexcept { size_t res{}; return res; }

#define REGISTER_TYPE(Type, Index)                                              \
    constexpr std::size_t type_to_id(identity<Type>) noexcept { return Index; } \
    constexpr auto id_to_type( std::integral_constant<std::size_t, Index > ) noexcept { Type res{}; return res; }  \
    /**/


// Register all base types here
    REGISTER_TYPE(unsigned short        , 1)
    REGISTER_TYPE(unsigned int          , 2)
    REGISTER_TYPE(unsigned long long    , 3)
    REGISTER_TYPE(signed char           , 4)
    REGISTER_TYPE(short                 , 5)
    REGISTER_TYPE(int                   , 6)
    REGISTER_TYPE(long long             , 7)
    REGISTER_TYPE(unsigned char         , 8)
    REGISTER_TYPE(char                  , 9)
    REGISTER_TYPE(wchar_t               , 10)
    REGISTER_TYPE(long                  , 11)
    REGISTER_TYPE(unsigned long         , 12)
    REGISTER_TYPE(void*                 , 13)
    REGISTER_TYPE(const void*           , 14)
    REGISTER_TYPE(char16_t              , 15)
    REGISTER_TYPE(char32_t              , 16)
    REGISTER_TYPE(float                 , 17)
    REGISTER_TYPE(double                , 18)
    REGISTER_TYPE(long double           , 19)


    constexpr std::size_t native_types_mask = 255;
    constexpr std::size_t bits_per_extension = 2;
    constexpr std::size_t native_ptr_type = (
            static_cast<std::size_t>(1)
                    << static_cast<std::size_t>(sizeof(std::size_t) * 8 - bits_per_extension)
    );
    constexpr std::size_t native_const_ptr_type = (
            static_cast<std::size_t>(2)
                    << static_cast<std::size_t>(sizeof(std::size_t) * 8 - bits_per_extension)
    );

    template <class Type>
    constexpr std::size_t type_to_id_extension_apply(identity<Type>, std::size_t ext) noexcept {
        constexpr auto unptr = type_to_id(identity<Type>{});
        constexpr auto native_id = (unptr & native_types_mask);
        constexpr auto extensions = (unptr & ~native_types_mask);
        static_assert(
                !((extensions >> bits_per_extension) & native_types_mask),
                "max extensions reached"
        );
        return (extensions >> bits_per_extension) | native_id | ext;
    }


    template <class Type>
    constexpr std::size_t type_to_id(identity<Type*>) noexcept {
        return type_to_id_extension_apply(identity<Type>{}, native_ptr_type);
    }

    template <class Type>
    constexpr std::size_t type_to_id(identity<const Type*>) noexcept {
        return type_to_id_extension_apply(identity<Type>{}, native_const_ptr_type);
    }

    template <std::size_t Index>
    constexpr auto id_to_type(
            std::integral_constant<std::size_t, Index >,
            typename std::enable_if< !!(Index & native_const_ptr_type) >::type* = 0) noexcept;


    template <std::size_t Index>
    constexpr auto id_to_type(
            std::integral_constant<std::size_t, Index >,
            typename std::enable_if< !!(Index & native_ptr_type) >::type* = 0) noexcept
    {
        auto t = id_to_type(
                std::integral_constant<
                        std::size_t,
                        ((Index & ~native_types_mask) << 2) | (Index & native_types_mask)
                >{}
        );
        decltype(t)* res{};
        return res;
    }

    template <std::size_t Index>
    constexpr auto id_to_type(
            std::integral_constant<std::size_t, Index >,
            typename std::enable_if< !!(Index & native_const_ptr_type) >::type*) noexcept
    {
        auto t = id_to_type(
                std::integral_constant<
                        std::size_t,
                        ((Index & ~native_types_mask) << 2) | (Index & native_types_mask)
                >{}
        );

        const decltype(t)* res{};
        return res;
    }


    template <std::size_t I>
    struct ubiq {
        std::size_t* ref_;

        template <class Type>
        constexpr operator Type() const noexcept {
            ref_[I] = type_to_id(identity<Type>{});
            return Type{};
        }
    };

    template <class T, std::size_t... I>
    constexpr auto type_to_array_of_type_ids(std::size_t* types) noexcept -> decltype(T{ ubiq<I>{types}... }) {
        return T{ ubiq<I>{types}... };
    }

    template <class T, std::size_t I0, std::size_t... I>
    constexpr auto detect_fields_count_and_type_ids(std::size_t* types, std::index_sequence<I0, I...>) noexcept
    -> decltype( type_to_array_of_type_ids<T, I0, I...>(types) )
    {
        return type_to_array_of_type_ids<T, I0, I...>(types);
    }

    template <class T, std::size_t... I>
    constexpr T detect_fields_count_and_type_ids(std::size_t* types, std::index_sequence<I...>) noexcept {
        return detect_fields_count_and_type_ids<T>(types, std::make_index_sequence<sizeof...(I) - 1>{});
    }

    template <class T>
    constexpr T detect_fields_count_and_type_ids(std::size_t*, std::index_sequence<>) noexcept {
        static_assert(!!sizeof(T), "Failed for unknown reason");
        return T{};
    }


    template <class T>
    constexpr auto fields_count_and_type_ids_with_zeros() noexcept {
        static_assert(std::is_trivial<T>::value, "Not applyable");
        array<std::size_t, sizeof(T)> types{};
        detect_fields_count_and_type_ids<T>(types.data, std::make_index_sequence<sizeof(T)>{});
        return types;
    }


    template <class T>
    constexpr auto array_of_type_ids() noexcept {
        constexpr auto types = fields_count_and_type_ids_with_zeros<T>();
        constexpr std::size_t count = count_nonzeros(types);
        array<std::size_t, count> res{};
        for (std::size_t i = 0; i < count; ++i) {
            res.data[i] = types.data[i];
        }

        return res;
    }


    template <class T, std::size_t... I>
    constexpr auto array_of_type_ids_to_index_sequence(std::index_sequence<I...>) noexcept {
        constexpr auto a = array_of_type_ids<T>();
        return std::index_sequence< get<I>(a)...>{};
    }


//    template <std::size_t... I>
//    constexpr auto as_tuple_impl(std::index_sequence<I...>) noexcept {
//        return std::tuple< decltype( id_to_type(std::integral_constant<std::size_t, I>{}) )... >{};
//    }
//
//    template <class T>
//    constexpr auto as_tuple() noexcept {
//        static_assert(std::is_pod<T>::value, "Not applyable");
//        constexpr auto res = as_tuple_impl(
//                array_of_type_ids_to_index_sequence<T>(
//                        std::make_index_sequence< decltype(array_of_type_ids<T>())::size() >()
//                )
//        );
//
//        static_assert(sizeof(res) == sizeof(T), "sizes check failed");
//        static_assert(
//                std::alignment_of<decltype(res)>::value == std::alignment_of<T>::value,
//                "alignment check failed"
//        );
//
//        return res;
//    }
    template <class T, std::size_t... I>
    constexpr auto as_tuple_impl(std::index_sequence<I...>) noexcept {
        constexpr auto a = array_of_type_ids<T>();
        return sequence_tuple::tuple<
                decltype(id_to_type(std::integral_constant<std::size_t, I>{}))...
        >{};
    }

    template <class T>
    constexpr sequence_tuple::tuple<> as_tuple_impl(std::index_sequence<>) noexcept {
        return sequence_tuple::tuple<>{};
    }

    template <class T>
    constexpr auto as_tuple() noexcept {
        typedef typename std::remove_cv<T>::type type;

        static_assert(std::is_pod<type>::value, "Not applyable");
        static_assert(!std::is_reference<type>::value, "Not applyable");
        constexpr auto res = as_tuple_impl<type>(
                array_of_type_ids_to_index_sequence<T>(
                        std::make_index_sequence< decltype(array_of_type_ids<T>())::size() >()
                )
        );

        static_assert(
                std::alignment_of<decltype(res)>::value == std::alignment_of<type>::value,
                "Alignment check failed, probably your structure has user-defined alignment for the whole structure or for some of the fields."
        );
        static_assert(sizeof(res) == sizeof(type), "Size check failed, probably your structure has bitfields or user-defined alignment.");

        return res;
    }
} // namespace detail

template <std::size_t I, class T>
decltype(auto) get(const T& val) noexcept {
//    decltype(detail::as_tuple<T>())* t1 = (decltype(detail::as_tuple<T>())*)(&val);
    auto t = reinterpret_cast<const decltype(detail::as_tuple<T>())*>( std::addressof(val) );
    return get<I>(*t);
}

template <class T>
constexpr std::size_t tuple_size() noexcept {
    return decltype(detail::as_tuple<T>())::size_v;
}


////////////////////////////////////////////////////////////////////

#include <cstdio>
#include  <typeindex>
#include <memory>
#include <boost/type_index.hpp>

struct foo {
    unsigned char i0;
    unsigned int i1;
    unsigned short i2;
    unsigned long long i3;
    unsigned char ar[2];
    int q;
    std::size_t w;
    int* p1;
    const void* p2;
    int const**const**** p_crazy;
    const double d;
};

template <std::size_t I, class T>
void print(T& f) {
    printf(
            "%lu\t\t%s\n",
            (std::size_t)get<I>(f),
            boost::typeindex::type_id<decltype(get<I>(f))>().pretty_name().c_str()
    );
}

int main() {
    foo f {10, 11, 12, 13, {14, 15}, 16, 17, 0, 0, 0, 30.0};
    print<0>(f);  print<1>(f);  print<2>(f);
    print<3>(f);  print<4>(f);  print<5>(f);
    print<6>(f);  print<7>(f);  print<8>(f);
    print<9>(f);  print<10>(f); print<11>(f);
    static_assert(tuple_size<foo>() == 12, "failed tuple size check");
    std::cout<<"finish"<<std::endl;
}
