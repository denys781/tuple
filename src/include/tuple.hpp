#pragma once

#include <utility>

namespace experimental
{
    template <typename Head, typename... Tail>
    struct tuple
    {
        Head m_value;
        tuple<Tail...> m_others;

        template <typename UHead, typename... UTail>
        tuple(UHead&& value, UTail&&... others)
            : m_value(std::forward<UHead>(value))
            , m_others(std::forward<UTail>(others)...)
        { }
    };

    template <typename Head>
    struct tuple <Head>
    {
        Head m_value;

        template <typename UHead>
        tuple(UHead&& value)
            : m_value(std::forward<UHead>(value))
        { }
    };

    namespace details
    {
        template <typename T>
        struct unwrap_refwrapper
        {
            using type = T;
        };

        template <typename T>
        struct unwrap_refwrapper<std::reference_wrapper<T>>
        {
            using type = T&;
        };

        template <typename T>
        using unwrap_refwrapper_t = typename unwrap_refwrapper<std::decay_t<T>>::type;
    }

    template <typename... Types>
    tuple<details::unwrap_refwrapper_t<Types>...> make_tuple(Types&&... args)
    {
        return { std::forward<Types>(args)... };
    }

    namespace details
    {
        template <std::size_t I, typename Head, typename... Tail>
        struct nth_type : nth_type<I - 1u, Tail...>
        {
            static_assert(I < sizeof...(Tail) + 1u, "type index is out of range");
        };

        template <typename Head, typename... Tail>
        struct nth_type <0u, Head, Tail...>
        {
            using type = Head;
        };

        template <size_t I>
        struct get_impl
        {
            template <typename... Types>
            static typename nth_type<I, Types...>::type& get(tuple<Types...>& t)
            {
                return get_impl<I - 1u>::get(t.m_others);
            }
        };

        template <>
        struct get_impl<0u>
        {
            template <typename Head, typename... Types>
            static Head& get(tuple<Head, Types...>& t)
            {
                return t.m_value;
            }
        };
    }

    template <std::size_t I, typename... Types>
    typename details::nth_type<I, Types...>::type& get(tuple<Types...>& t)
    {
        return details::get_impl<I>::get(t);
    }
}
