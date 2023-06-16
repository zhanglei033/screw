#pragma once
#ifndef _STATIC_STRING_H_
#define _STATIC_STRING_H_
#include "Platform.h"
#include "StringView.h"
#include <ostream>
namespace screw {
//#if STD_HAS_CXX20

//#elif STD_HAS_CXX17

//#elif STD_HAS_CXX14
template <class Elem, size_t N>
struct StaticBasicString
{
    using value_type = Elem;
    using size_type  = size_t;

    DECL_STATIC_CONSTEXPR size_type num = N;
    value_type m_elems[N];

    template <size_type... I>
    DECL_CONSTEXPR11 StaticBasicString(const value_type (&elems)[N], std::index_sequence<I...>) DECL_NOEXCEPT
        : m_elems{elems[I]...}
    {
    }

    template <class ElemType, size_t ElemNum>
    DECL_STATIC_CONSTEXPR auto Create(const ElemType (&elems)[ElemNum]) DECL_NOEXCEPT
    {
        return StaticBasicString<ElemType, ElemNum>(elems, std::make_index_sequence<ElemNum>{});
    }

    template <size_t Prefix, size_t Suffix>
    DECL_CONSTEXPR11 auto Substr() const DECL_NOEXCEPT
    {
        constexpr size_t Num = num - Prefix - Suffix - 1;
        return SubstrImpl<Prefix>(std::make_index_sequence<Num>{});
    }

    friend std::ostream& operator<<(std::ostream& os, const StaticBasicString& str)
    {
        using ostr_type                   = std::basic_ostream<Elem, std::char_traits<Elem>>;
        typename ostr_type::iostate state = ostr_type::goodbit;

        size_t pad;
        if (os.width() <= 0 || static_cast<size_t>(os.width()) <= num)
        {
            pad = 0;
        }
        else
        {
            pad = static_cast<size_t>(os.width()) - num;
        }

        const typename ostr_type::sentry ok(os);

        if (!ok)
        {
            state |= ostr_type::badbit;
        }
        else
        {
            try
            {
                if ((os.flags() & ostr_type::adjustfield) != ostr_type::left)
                {
                    for (; 0 < pad; --pad)
                    {
                        if (std::char_traits<Elem>::eq_int_type(std::char_traits<Elem>::eof(), os.rdbuf()->sputc(os.fill())))
                        {
                            state |= ostr_type::badbit;
                            break;
                        }
                    }
                }

                if (state == ostr_type::goodbit && os.rdbuf()->sputn(str.m_elems, static_cast<std::streamsize>(num)) != static_cast<std::streamsize>(num))
                {
                    state |= ostr_type::badbit;
                }
                else
                {
                    for (; 0 < pad; --pad)
                    {
                        if (std::char_traits<Elem>::eq_int_type(std::char_traits<Elem>::eof(), os.rdbuf()->sputc(os.fill())))
                        {
                            state |= ostr_type::badbit;
                            break;
                        }
                    }
                }

                os.width(0);
            } catch (...)
            {
                os.setstate(ostr_type::badbit, true);
            }
        }

        os.setstate(state);
        return os;
    }

    std::basic_string<value_type> ToString() const
    {
        return std::basic_string<value_type>(m_elems, num);
    }

    std::basic_string_view<value_type, std::char_traits<value_type>> ToStringView() const
    {
        return std::basic_string_view<value_type>(m_elems, num);
    }

private:
    template <size_t Prefix, size_type... I>
    DECL_CONSTEXPR11 auto SubstrImpl(std::index_sequence<I...>) const DECL_NOEXCEPT
    {
        return Create({m_elems[I + Prefix]..., '\0'});
    }
};

using StatiString = StaticBasicString<char, 0>;
//#endif // !STD

} // namespace screw
#endif // !_STATIC_STRING_H_
