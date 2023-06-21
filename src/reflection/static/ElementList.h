#pragma once
#ifndef _ELEMENT_LIST_H_
#define _ELEMENT_LIST_H_
#include "Element.h"
#include <tuple>
namespace screw {
namespace reflection {
template <class Element>
DECL_STATIC_CONSTEXPR bool is_empty_element(Element);
namespace detail {
template <class... Elements>
struct ElementList
{
    using element_list = std::meta_list<Elements...>;

    DECL_STATIC_CONSTEXPR size_t element_size = std::meta_list_size_v<element_list>;
    template <class NameType>
    DECL_STATIC_CONSTEXPR bool is_contains_v = std::is_any_of_v<NameType, typename ElementInfo<typename Elements::element_traits>::name_type...>;

    template <class Callable>
    DECL_STATIC_CONSTEXPR void ForEach(Callable&& caller) DECL_NOEXCEPT
    {
        ForEach<0, element_size>(std::forward<Callable>(caller));
    }

    template <class Type, class Callable>
    DECL_STATIC_CONSTEXPR void ForEachMemberVar(Type&& obj, Callable&& caller) DECL_NOEXCEPT
    {
        ForEachMemberVar<0, element_size>(std::forward<Type>(obj), std::forward<Callable>(caller));
    }

    template <class NameType>
    DECL_STATIC_CONSTEXPR auto Find(NameType name)
    {
        return Find<0, element_size, NameType>();
    }

private:
    template <size_t CurIdx, class Type, class Callable>
    DECL_STATIC_CONSTEXPR void Invoke(Type&& obj, Callable&& caller) DECL_NOEXCEPT
    {
        using element_type = std::meta_list_at_t<element_list, CurIdx>;
        using traits_type  = typename element_type::traits_type;
        using element_info = ElementInfo<traits_type>;

        constexpr bool isNeedMemberVar = !std::is_same_v<Type, std::nullptr_t>;
        if constexpr (isNeedMemberVar && !element_info::is_member_variable_v)
        {
            return;
        }
        else
        {
            element_type elem;
            if constexpr (!isNeedMemberVar) // 不是遍历成员变量值时，仅传出元素变量
            {
                std::forward<Callable>(caller)(std::forward<element_type>(elem));
            }
            else if constexpr (element_info::is_member_variable_static_v)
            {
                std::forward<Callable>(caller)(std::forward<element_type>(elem), elem.Invoke());
            }
            else
            {
                std::forward<Callable>(caller)(std::forward<element_type>(elem), elem.Invoke(std::forward<Type>(obj)));
            }
        }
    }

    template <size_t CurIdx, size_t MaxIdx, class Callable>
    DECL_STATIC_CONSTEXPR void ForEach(Callable&& caller) DECL_NOEXCEPT
    {
        if constexpr (CurIdx < MaxIdx)
        {
            using element_type = std::meta_list_at_t<element_list, CurIdx>;
            using traits_type  = typename element_type::traits_type;
            using element_info = ElementInfo<traits_type>;

            Invoke<CurIdx>(std::nullptr_t{}, std::forward<Callable>(caller));
            if constexpr (element_info::is_member_v) // 遍历的元素类型是成员时，需要访问基类的的成员
            {
                using type_info       = typename traits_type::type_info;
                using bases_type_info = typename type_info::bases_type_info;
                ForEachBases<bases_type_info, 0, std::meta_list_size_v<bases_type_info>>(std::forward<Callable>(caller));
            }
            ForEach<CurIdx + 1, MaxIdx>(std::forward<Callable>(caller));
        }
    }

    template <class BasesTypeInfo, size_t CurIdx, size_t MaxIdx, class Callable>
    DECL_STATIC_CONSTEXPR void ForEachBases(Callable&& caller) DECL_NOEXCEPT
    {
        if constexpr (CurIdx < MaxIdx)
        {
            using base_type_info = std::meta_list_at_t<BasesTypeInfo, CurIdx>;
            base_type_info::ForEachElement(std::forward<Callable>(caller));
            ForEachBases<BasesTypeInfo, CurIdx + 1, MaxIdx>(std::forward<Callable>(caller));
        }
    }

    template <size_t CurIdx, size_t MaxIdx, class Type, class Callable>
    DECL_STATIC_CONSTEXPR void ForEachMemberVar(Type&& obj, Callable&& caller) DECL_NOEXCEPT
    {
        if constexpr (CurIdx < MaxIdx)
        {
            using element_type = std::meta_list_at_t<element_list, CurIdx>;
            using traits_type  = typename element_type::traits_type;
            using element_info = ElementInfo<traits_type>;
         
            Invoke<CurIdx>(std::forward<Type>(obj), std::forward<Callable>(caller));
            if constexpr (element_info::is_member_v) // 遍历的元素类型是成员时，需要访问基类的的成员
            {
                using type_info       = typename traits_type::type_info;
                using bases_type_info = typename type_info::bases_type_info;
                ForEachBases<bases_type_info, 0, std::meta_list_size_v<bases_type_info>>(std::forward<Type>(obj), std::forward<Callable>(caller));
            }
            ForEachMemberVar<CurIdx + 1, MaxIdx>(std::forward<Type>(obj), std::forward<Callable>(caller));
        }
    }

    template <class BasesTypeInfo, size_t CurIdx, size_t MaxIdx, class Type, class Callable>
    DECL_STATIC_CONSTEXPR auto ForEachBases(Type&& obj, Callable&& caller) DECL_NOEXCEPT
    {
        if constexpr (CurIdx < MaxIdx)
        {
            using base_type_info = std::meta_list_at_t<BasesTypeInfo, CurIdx>;
            using base_type      = typename base_type_info::type;
            base_type_info::ForEachMemberVar(std::forward<base_type>(obj), std::forward<Callable>(caller));
            ForEachBases<BasesTypeInfo, CurIdx + 1, MaxIdx>(std::forward<Callable>(caller));
        }
    }

    template <size_t CurIdx, size_t MaxIdx, class NameType>
    DECL_STATIC_CONSTEXPR auto Find()
    {
        if constexpr (CurIdx < MaxIdx)
        {
            using element_type = std::meta_list_at_t<element_list, CurIdx>;
            using traits_type  = typename element_type::traits_type;
            using element_info = ElementInfo<traits_type>;

            if constexpr (std::is_same_v<NameType, typename element_info::name_type>)
            {
                return element_type{};
            }
            else
            {
                constexpr auto elem1 = Find<CurIdx + 1, MaxIdx, NameType>();
                if constexpr (!is_empty_element(elem1))
                {
                    return elem1;
                }
                else
                {
                    if constexpr (element_info::is_member_v) // 遍历的元素类型是成员时，需要访问基类的的成员
                    {
                        using type_info       = typename traits_type::type_info;
                        using bases_type_info = typename type_info::bases_type_info;
                        return FindBases<bases_type_info, 0, std::meta_list_size_v<bases_type_info>, NameType>();
                    }
                    else
                    {
                        return detail::EmptyElement{};
                    }
                }
            }
        }
        else
        {
            return detail::EmptyElement{};
        }
    }

    template <class BasesTypeInfo, size_t CurIdx, size_t MaxIdx, class NameType>
    DECL_STATIC_CONSTEXPR auto FindBases() DECL_NOEXCEPT
    {
        if constexpr (CurIdx < MaxIdx)
        {
            using base_type_info = std::meta_list_at_t<BasesTypeInfo, CurIdx>;
            constexpr auto elem  = base_type_info::FindElement(NameType{});
            if constexpr (!is_empty_element(elem))
            {
                return elem;
            }
            else
            {
                return FindBases<BasesTypeInfo, CurIdx + 1, MaxIdx, NameType>();
            }
        }
        else
        {
            return detail::EmptyElement{};
        }
    }
};
} // namespace detail
}}     // namespace screw::reflection
#endif // !_ELEMENT_LIST_H_
