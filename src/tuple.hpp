#pragma once

#include <utility>
#include <type_traits>

namespace experimental
{
    namespace details
    {
        template <std::size_t> struct get_impl;
    }

    template <class _First, class... _Others>
    class tuple
    {
        _First m_value;
        tuple<_Others...> m_others;
    public:
        template <std::size_t> friend struct details::get_impl;

        template <class _First2, class... _Others2,
            class = std::enable_if_t<sizeof...(_Others) == sizeof...(_Others2)
                && std::is_constructible<_First, _First2>::value>>
        tuple(_First2&& value, _Others2&&... others)
            : m_value(std::forward<_First2>(value))
            , m_others(std::forward<_Others2>(others)...)
        { }
    };

    template <class _First>
    class tuple<_First>
    {
        _First m_value;
    public:
        friend struct details::get_impl<0>;

        template <class _First2,
            class = std::enable_if_t<std::is_constructible<_First, _First2>::value>>
        tuple(_First2&& value)
            : m_value(std::forward<_First2>(value))
        { }
    };

    namespace details
    {
        template <class _Ty>
        struct unwrap_refwrapper
        {
            using type = _Ty;
        };

        template <class _Ty>
        struct unwrap_refwrapper<std::reference_wrapper<_Ty>>
        {
            using type = _Ty&;
        };

        template <class _Ty>
        using unwrap_refwrapper_t = typename unwrap_refwrapper<std::decay_t<_Ty>>::type;
    }

    template <class... _Types>
    decltype(auto) make_tuple(_Types&&... args)
    {
        using tuple_t = tuple<details::unwrap_refwrapper_t<_Types>...>;
        return tuple_t(std::forward<_Types>(args)...);
    }

    namespace details
    {
        template <std::size_t I, class _First, class... _Others>
        struct nth_type : nth_type<I - 1, _Others...>
        {
            static_assert(I < sizeof...(_Others) + 1, "type index is out of range");
        };

        template <class _First, class... _Others>
        struct nth_type<0, _First, _Others...>
        {
            using type = _First;
        };

        template <std::size_t I>
        struct get_impl
        {
            template <class... _Types>
            static typename nth_type<I, _Types...>::type& get(tuple<_Types...>& t)
            {
                return get_impl<I - 1>::get(t.m_others);
            }
        };

        template <>
        struct get_impl<0>
        {
            template <class _First, class... _Others>
            static _First& get(tuple<_First, _Others...>& t)
            {
                return t.m_value;
            }
        };
    }

    template <std::size_t I, class... _Types>
    typename details::nth_type<I, _Types...>::type& get(tuple<_Types...>& t)
    {
        return details::get_impl<I>::get(t);
    }
}