#include "base/StringView.h"
#include "googletest/gtest/gtest.h"

using namespace std;

void test_case_operator_dereference_value_initialized_iterator()
{
    string_view::iterator it; // note: for IDL to work correctly, default init and value init are equivalent

    //(void)*it;                // cannot dereference value-initialized string_view iterator
}

void test_case_operator_dereference_end_iterator()
{
    string_view sv("text");
    string_view::iterator it = sv.end();

    //(void) *it; // cannot dereference end string_view iterator
}

void test_case_operator_arrow_value_initialized_iterator()
{
    string_view::iterator it;
    //(void)it.operator->(); // cannot dereference value-initialized string_view iterator
}

void test_case_operator_arrow_end_iterator()
{
    string_view sv("text");
    string_view::iterator it = sv.end();

    //(void)it.operator->(); // cannot dereference end string_view iterator
}

void test_case_operator_preincrement_value_initialized_iterator()
{
    string_view::iterator it;

    //++it; // cannot increment value-initialized string_view iterator
}

void test_case_operator_preincrement_off_end()
{
    string_view sv("text");
    string_view::iterator it = sv.begin();

    // for (size_t idx = 0; idx < 5; ++idx)
    //{
    //     ++it; // cannot increment string_view iterator past end
    // }
}

void test_case_operator_predecrement_value_initialized_iterator()
{
    string_view::iterator it;

    //--it; // cannot decrement value-initialized string_view iterator
}

void test_case_operator_predecrement_before_begin()
{
    string_view sv("text");
    string_view::iterator it = sv.begin();

    // --it; // cannot decrement string_view iterator before begin
}

void test_case_operator_advance_value_initialized_iterator()
{
    string_view::iterator it;

    // it += 1; // cannot seek value-initialized string_view iterator
}

void test_case_operator_advance_value_initialized_iterator_zero()
{
    string_view::iterator it;
    it += 0; // OK
}

void test_case_operator_advance_before_begin()
{
    string_view sv("text");
    string_view::iterator it = sv.begin();

    // it += -1; // cannot seek string_view iterator before begin
}

void test_case_operator_advance_after_end()
{
    string_view sv("text");
    string_view::iterator it = sv.begin();

    // it += 5; // cannot seek string_view iterator after end
}

void test_case_operator_retreat_value_initialized_iterator()
{
    string_view::iterator it;
    // it -= 1; // cannot seek value-initialized string_view iterator
}

void test_case_operator_retreat_value_initialized_iterator_zero()
{
    string_view::iterator it;
    it -= 0; // OK
}

void test_case_operator_retreat_before_begin()
{
    string_view sv("text");
    string_view::iterator it = sv.begin();

    // it -= 1; // cannot seek string_view iterator before begin
}

void test_case_operator_retreat_after_end()
{
    string_view sv("text");
    string_view::iterator it = sv.begin();

    // it -= -5; // cannot seek string_view iterator after end
}

void test_case_operator_subtract_incompatible_different_views()
{
    string_view sv1("text");
    string_view sv2("text2");

    //(void)(sv1.begin() - sv2.begin()); // cannot subtract incompatible string_view iterators
}

void test_case_operator_subtract_incompatible_value_initialized()
{
    string_view sv1("text");

    // (void)(sv1.begin() - string_view::iterator{}); // cannot subtract incompatible string_view iterators
}

void test_case_operator_equal_incompatible_different_views()
{
    string_view sv1("text");
    string_view sv2("text2");

    // (void)(sv1.begin() == sv2.begin()); // cannot compare incompatible string_view iterators for equality
}

void test_case_operator_equal_incompatible_value_initialized()
{
    string_view sv1("text");

    // (void)(sv1.begin() == string_view::iterator{}); // cannot compare incompatible string_view iterators for equality
}

void test_case_operator_less_incompatible_different_views()
{
    string_view sv1("text");
    string_view sv2("text2");

    // (void)(sv1.begin() < sv2.begin()); // cannot compare incompatible string_view iterators
}

void test_case_operator_less_incompatible_value_initialized()
{
    string_view sv1("text");

    //(void)(sv1.begin() < string_view::iterator{}); // cannot compare incompatible string_view iterators
}

void test_case_operator_subscript_out_of_range()
{
    string_view sv("text");

    //(void)sv[4]; // string_view subscript out of range
}

void test_case_front_empty()
{
    string_view sv;

    // (void)sv.front(); // cannot call front on empty string_view
}

void test_case_back_empty()
{
    string_view sv;

    // (void)sv.back(); // cannot call back on empty string_view
}

void test_case_remove_prefix_too_large()
{
    string_view sv("text");
    // sv.remove_prefix(5); // cannot remove prefix longer than total size
}

void test_case_remove_prefix_zero()
{
    string_view sv;
    sv.remove_prefix(0); // OK
}

void test_case_remove_suffix_too_large()
{
    string_view sv("text");

    // sv.remove_suffix(5); // cannot remove suffix longer than total size
}

void test_case_remove_suffix_zero()
{
    string_view sv;
    sv.remove_suffix(0); // OK
}

void test_case_remove_prefix_incompatible()
{
    string_view sv("text");
    auto old_range = sv.begin();
    sv.remove_prefix(1);
    auto new_range = sv.begin();
    // (void)(old_range == new_range); // cannot compare incompatible string_view iterators for equality
}

void test_case_remove_suffix_incompatible()
{
    string_view sv("text");
    auto old_range = sv.begin();
    sv.remove_suffix(1);
    auto new_range = sv.begin();
    // (void)(old_range == new_range); // cannot compare incompatible string_view iterators for equality
}

void test_case_null_constructor()
{
#pragma warning(suppress : 6387) // yay PREfast catches this mistake at compile time!
    string_view sv(nullptr, 1);  // non-zero size null string_view
    (void)sv;
}

void test_case_null_constructor_zero()
{
    string_view sv(nullptr, 0); // OK
    (void)sv;
}

TEST(string_view_test, test1)
{
    test_case_operator_dereference_value_initialized_iterator();
    test_case_operator_dereference_end_iterator();
    test_case_operator_arrow_value_initialized_iterator();
    test_case_operator_arrow_end_iterator();
    test_case_operator_preincrement_value_initialized_iterator();
    test_case_operator_preincrement_off_end();
    test_case_operator_predecrement_value_initialized_iterator();
    test_case_operator_predecrement_before_begin();
    test_case_operator_advance_value_initialized_iterator();
    test_case_operator_advance_before_begin();
    test_case_operator_advance_after_end();
    test_case_operator_retreat_value_initialized_iterator();
    test_case_operator_retreat_before_begin();
    test_case_operator_retreat_after_end();
    test_case_operator_subtract_incompatible_different_views();
    test_case_operator_subtract_incompatible_value_initialized();
    test_case_operator_equal_incompatible_different_views();
    test_case_operator_equal_incompatible_value_initialized();
    test_case_operator_less_incompatible_different_views();
    test_case_operator_less_incompatible_value_initialized();
    test_case_operator_subscript_out_of_range();
    test_case_front_empty();
    test_case_back_empty();
    test_case_remove_prefix_too_large();
    test_case_remove_prefix_zero();
    test_case_remove_suffix_too_large();
    test_case_remove_suffix_zero();

    test_case_remove_prefix_incompatible();
    test_case_remove_suffix_incompatible();

    test_case_null_constructor();
    test_case_null_constructor_zero();
}

struct constexpr_char_traits
{
    typedef char char_type;
    typedef long int_type;
    typedef long pos_type;
    typedef long off_type;
    typedef mbstate_t state_type;

    static constexpr int compare(const char* first1, const char* first2, size_t count)
    {
        for (; 0 < count; --count, ++first1, ++first2)
        {
            if (!eq(*first1, *first2))
            {
                return lt(*first1, *first2) ? -1 : +1;
            }
        }

        return 0;
    }

    static constexpr size_t length(const char* first)
    {
        size_t count = 0;
        for (; !eq(*first, char()); ++first)
        {
            ++count;
        }

        return count;
    }

    static constexpr char* copy(char* first1, const char* first2, size_t count)
    {
        char* next = first1;
        for (; 0 < count; --count, ++next, ++first2)
        {
            assign(*next, *first2);
        }
        return first1;
    }

    static constexpr char* _Copy_s(char* first1, size_t, const char* first2, size_t count)
    {
        // let's just pretend :)
        return copy(first1, first2, count);
    }

    static constexpr const char* find(const char* first, size_t count, const char ch)
    {
        for (; 0 < count; --count, ++first)
        {
            if (eq(*first, ch))
            {
                return first;
            }
        }

        return nullptr;
    }

    static constexpr char* move(char* first1, const char* first2, size_t count)
    {
        char* next = first1;
        if (first2 < next && next < first2 + count)
        {
            for (next += count, first2 += count; 0 < count; --count)
            {
                assign(*--next, *--first2);
            }
        }
        else
        {
            for (; 0 < count; --count, ++next, ++first2)
            {
                assign(*next, *first2);
            }
        }
        return first1;
    }

    static constexpr char* assign(char* first, size_t count, const char ch)
    {
        char* next = first;
        for (; 0 < count; --count, ++next)
        {
            assign(*next, ch);
        }

        return first;
    }

    static constexpr void assign(char& left, const char right) noexcept
    {
        left = right;
    }

    static constexpr bool eq(const char left, const char right) noexcept
    {
        return left == right;
    }

    static constexpr bool lt(const char left, const char right) noexcept
    {
        return left < right;
    }

    static constexpr char to_char_type(const int_type meta) noexcept
    {
        return static_cast<char>(meta);
    }

    static constexpr int_type to_int_type(const char ch) noexcept
    {
        return ch;
    }

    static constexpr bool eq_int_type(const int_type left, const int_type right) noexcept
    {
        return left == right;
    }

    static constexpr int_type not_eof(const int_type meta) noexcept
    {
        return meta != eof() ? meta : !eof();
    }

    static constexpr int_type eof() noexcept
    {
        return EOF;
    }
};

constexpr auto ptrdiff_max = numeric_limits<ptrdiff_t>::max();

template <typename Expected, typename Fn>
void assert_throws(Fn fn)
{
    try
    {
        (void)fn();
        abort();
    } catch (const Expected&)
    {
    }
}

using c_string_view = basic_string_view<char, constexpr_char_traits>;

struct conversion_to_string_view
{
    operator string_view()
    {
        return string_view{};
    }
};

struct nofail_conversion_to_string_view
{
    operator string_view() noexcept
    {
        return string_view{};
    }
};

struct evil_conversion_to_string_view_rvalue_only
{
    operator string_view() && noexcept
    {
        return string_view{};
    }

    operator string_view() const&
    {
        return string_view{};
    }
};

struct evil_conversion_to_string_view_lvalue_only
{
    operator string_view() &&
    {
        return string_view{};
    }

    operator string_view() const& noexcept
    {
        return string_view{};
    }
};
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#endif // __clang__
const evil_conversion_to_string_view_rvalue_only convert_rvalue_only{};
const evil_conversion_to_string_view_lvalue_only convert_lvalue_only{};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif // __clang__

static_assert(is_trivially_copyable_v<string_view>, "");
static_assert(is_trivially_copyable_v<wstring_view>, "");

// Implied by N4901 [class.prop]/1
static_assert(is_trivially_move_constructible_v<string_view>, "");
static_assert(is_trivially_copy_constructible_v<string_view>, "");
static_assert(is_trivially_move_assignable_v<string_view>, "");
static_assert(is_trivially_copy_assignable_v<string_view>, "");
static_assert(is_trivially_destructible_v<string_view>, "");

static_assert(is_trivially_move_constructible_v<wstring_view>, "");
static_assert(is_trivially_copy_constructible_v<wstring_view>, "");
static_assert(is_trivially_move_assignable_v<wstring_view>, "");
static_assert(is_trivially_copy_assignable_v<wstring_view>, "");
static_assert(is_trivially_destructible_v<wstring_view>, "");

// noexcept assertions:
// (functions that explicitly throw have their throws tested and therefore have no static_asserts)
static_assert(noexcept(string_view{}), "default constructor not noexcept");
static_assert(noexcept(string_view{string_view{}}), "copy constructor not noexcept");
string_view g_example{"text", 2};

// the assignment operator is arguably noexcept in the WP (it is defaulted)
static_assert(noexcept(declval<string_view&>() = declval<string_view>()), "assignment operator not noexcept");
static_assert(noexcept(string_view{"text"}), "NTCTS constructor not noexcept");     // strengthened
static_assert(noexcept(string_view{"text", 2}), "buffer constructor not noexcept"); // strengthened
static_assert(noexcept(g_example.begin()), "begin not noexcept");
static_assert(noexcept(g_example.end()), "end not noexcept");
static_assert(noexcept(g_example.cbegin()), "cbegin not noexcept");
static_assert(noexcept(g_example.cend()), "cend not noexcept");
static_assert(noexcept(g_example.rbegin()), "rbegin not noexcept");
static_assert(noexcept(g_example.rend()), "rend not noexcept");
static_assert(noexcept(g_example.crbegin()), "crbegin not noexcept");
static_assert(noexcept(g_example.crend()), "crend not noexcept");
static_assert(noexcept(g_example.size()), "size not noexcept");
static_assert(noexcept(g_example.length()), "length not noexcept");
static_assert(noexcept(g_example.max_size()), "max_size not noexcept");
static_assert(noexcept(g_example.empty()), "empty not noexcept");
static_assert(noexcept(g_example[0]), "operator[] not noexcept"); // strengthened
// at throws out_of_range
static_assert(noexcept(g_example.front()), "front() not noexcept"); // strengthened
static_assert(noexcept(g_example.back()), "back() not noexcept");   // strengthened
static_assert(noexcept(g_example.data()), "data() not noexcept");
static_assert(noexcept(g_example.remove_prefix(1)), "remove_prefix() not noexcept"); // strengthened
static_assert(noexcept(g_example.remove_suffix(1)), "remove_suffix() not noexcept"); // strengthened

string_view g_example_swap_target{"text", 3};
static_assert(noexcept(g_example.swap(g_example_swap_target)), "swap() not noexcept");
// copy throws out_of_range
// substr throws out_of_range
static_assert(noexcept(g_example.compare(string_view{})), "compare(basic_string_view) not noexcept");
// compare(pos1, n1, basic_string_view) throws out_of_range
// compare(pos1, n1, basic_string_view, pos2, n2) throws out_of_range
static_assert(!noexcept(g_example.compare("literal")), "compare(const charT*) not noexcept (this is bad, char_traits)");
// compare(pos1, n1, const charT*) throws out_of_range and calls through char_traits
// compare(pos1, n1, const charT*, n2) throws out_of_range and calls through char_traits
static_assert(noexcept(g_example.find(string_view{})), "find(basic_string_view, offset) not noexcept");
static_assert(noexcept(g_example.find('c')), "find(charT, count) not noexcept");
static_assert(noexcept(g_example.find("text", 0, 0)), "find(const charT *, pos, n) not noexcept"); // strengthened
static_assert(noexcept(g_example.find("text")), "find(const charT *, pos) not noexcept");          // strengthened
static_assert(noexcept(g_example.rfind(string_view{})), "rfind(basic_string_view, offset) not noexcept");
static_assert(noexcept(g_example.rfind('c')), "rfind(charT, count) not noexcept");
static_assert(noexcept(g_example.rfind("text", 0, 0)), "rfind(const charT *, pos, n) not noexcept"); // strengthened
static_assert(noexcept(g_example.rfind("text")), "rfind(const charT *, pos) not noexcept");          // strengthened
static_assert(noexcept(g_example.find_first_of(string_view{})), "find_first_of(basic_string_view, offset) not noexcept");
static_assert(noexcept(g_example.find_first_of('c')), "find_first_of(charT, count) not noexcept");
static_assert(noexcept(g_example.find_first_of("text", 0, 0)), "find_first_of(const charT *, pos, n) not noexcept"); // strengthened
static_assert(noexcept(g_example.find_first_of("text")), "find_first_of(const charT *, pos) not noexcept");          // strengthened
static_assert(noexcept(g_example.find_last_of(string_view{})), "find_last_of(basic_string_view, offset) not noexcept");
static_assert(noexcept(g_example.find_last_of('c')), "find_last_of(charT, count) not noexcept");
static_assert(noexcept(g_example.find_last_of("text", 0, 0)), "find_last_of(const charT *, pos, n) not noexcept"); // strengthened
static_assert(noexcept(g_example.find_last_of("text")), "find_last_of(const charT *, pos) noexcept");              // strengthened
static_assert(noexcept(g_example.find_first_not_of(string_view{})), "find_first_not_of(basic_string_view, offset) not noexcept");
static_assert(noexcept(g_example.find_first_not_of('c')), "find_first_not_of(charT, count) not noexcept");
static_assert(noexcept(g_example.find_first_not_of("text", 0, 0)), "find_first_not_of(const charT *, pos, n) not noexcept"); // strengthened
static_assert(noexcept(g_example.find_first_not_of("text")), "find_first_not_of(const charT *, pos) not noexcept");          // strengthened
static_assert(noexcept(g_example.find_last_not_of(string_view{})), "find_last_not_of(basic_string_view, offset) not noexcept");
static_assert(noexcept(g_example.find_last_not_of('c')), "find_last_not_of(charT, count) not noexcept");
static_assert(noexcept(g_example.find_last_not_of("text", 0, 0)), "find_last_not_of(const charT *, pos, n) not noexcept"); // strengthened
static_assert(noexcept(g_example.find_last_not_of("text")), "find_last_not_of(const charT *, pos) noexcept");              // strengthened

static_assert(noexcept(string_view{} == string_view{}), "operator== not noexcept");
static_assert(!noexcept(conversion_to_string_view{} == string_view{}), "operator== left noexcept forward");
static_assert(noexcept(evil_conversion_to_string_view_rvalue_only{} == string_view{}), "operator== left noexcept forward");
static_assert(!noexcept(convert_rvalue_only == string_view{}), "operator== left noexcept forward");
static_assert(!noexcept(evil_conversion_to_string_view_lvalue_only{} == string_view{}), "operator== left noexcept forward");
static_assert(noexcept(convert_lvalue_only == string_view{}), "operator== left noexcept forward");
static_assert(!noexcept(string_view{} == conversion_to_string_view{}), "operator== right noexcept forward");
static_assert(noexcept(string_view{} == evil_conversion_to_string_view_rvalue_only{}), "operator== right noexcept forward");
static_assert(!noexcept(string_view{} == convert_rvalue_only), "operator== right noexcept forward");
static_assert(!noexcept(string_view{} == evil_conversion_to_string_view_lvalue_only{}), "operator== right noexcept forward");
static_assert(noexcept(string_view{} == convert_lvalue_only), "operator== right noexcept forward");

static_assert(noexcept(string_view{} != string_view{}), "operator!= not noexcept");
static_assert(!noexcept(conversion_to_string_view{} != string_view{}), "operator!= left noexcept forward");
static_assert(noexcept(evil_conversion_to_string_view_rvalue_only{} != string_view{}), "operator!= left noexcept forward");
static_assert(!noexcept(convert_rvalue_only != string_view{}), "operator!= left noexcept forward");
static_assert(!noexcept(evil_conversion_to_string_view_lvalue_only{} != string_view{}), "operator!= left noexcept forward");
static_assert(noexcept(convert_lvalue_only != string_view{}), "operator!= left noexcept forward");
static_assert(!noexcept(string_view{} != conversion_to_string_view{}), "operator!= right noexcept forward");
static_assert(noexcept(string_view{} != evil_conversion_to_string_view_rvalue_only{}), "operator!= right noexcept forward");
static_assert(!noexcept(string_view{} != convert_rvalue_only), "operator!= right noexcept forward");
static_assert(!noexcept(string_view{} != evil_conversion_to_string_view_lvalue_only{}), "operator!= right noexcept forward");
static_assert(noexcept(string_view{} != convert_lvalue_only), "operator!= right noexcept forward");

static_assert(noexcept(string_view{} < string_view{}), "operator< not noexcept");
static_assert(!noexcept(conversion_to_string_view{} < string_view{}), "operator< left noexcept forward");
static_assert(noexcept(evil_conversion_to_string_view_rvalue_only{} < string_view{}), "operator< left noexcept forward");
static_assert(!noexcept(convert_rvalue_only < string_view{}), "operator< left noexcept forward");
static_assert(!noexcept(evil_conversion_to_string_view_lvalue_only{} < string_view{}), "operator< left noexcept forward");
static_assert(noexcept(convert_lvalue_only < string_view{}), "operator< left noexcept forward");
static_assert(!noexcept(string_view{} < conversion_to_string_view{}), "operator< right noexcept forward");
static_assert(noexcept(string_view{} < evil_conversion_to_string_view_rvalue_only{}), "operator< right noexcept forward");
static_assert(!noexcept(string_view{} < convert_rvalue_only), "operator< right noexcept forward");
static_assert(!noexcept(string_view{} < evil_conversion_to_string_view_lvalue_only{}), "operator< right noexcept forward");
static_assert(noexcept(string_view{} < convert_lvalue_only), "operator< right noexcept forward");

static_assert(noexcept(string_view{} > string_view{}), "operator> not noexcept");
static_assert(!noexcept(conversion_to_string_view{} > string_view{}), "operator> left noexcept forward");
static_assert(noexcept(evil_conversion_to_string_view_rvalue_only{} > string_view{}), "operator> left noexcept forward");
static_assert(!noexcept(convert_rvalue_only > string_view{}), "operator> left noexcept forward");
static_assert(!noexcept(evil_conversion_to_string_view_lvalue_only{} > string_view{}), "operator> left noexcept forward");
static_assert(noexcept(convert_lvalue_only > string_view{}), "operator> left noexcept forward");
static_assert(!noexcept(string_view{} > conversion_to_string_view{}), "operator> right noexcept forward");
static_assert(noexcept(string_view{} > evil_conversion_to_string_view_rvalue_only{}), "operator> right noexcept forward");
static_assert(!noexcept(string_view{} > convert_rvalue_only), "operator> right noexcept forward");
static_assert(!noexcept(string_view{} > evil_conversion_to_string_view_lvalue_only{}), "operator> right noexcept forward");
static_assert(noexcept(string_view{} > convert_lvalue_only), "operator> right noexcept forward");

static_assert(noexcept(string_view{} <= string_view{}), "operator<= not noexcept");
static_assert(!noexcept(conversion_to_string_view{} <= string_view{}), "operator<= left noexcept forward");
static_assert(noexcept(evil_conversion_to_string_view_rvalue_only{} <= string_view{}), "operator<= left noexcept forward");
static_assert(!noexcept(convert_rvalue_only <= string_view{}), "operator<= left noexcept forward");
static_assert(!noexcept(evil_conversion_to_string_view_lvalue_only{} <= string_view{}), "operator<= left noexcept forward");
static_assert(noexcept(convert_lvalue_only <= string_view{}), "operator<= left noexcept forward");
static_assert(!noexcept(string_view{} <= conversion_to_string_view{}), "operator<= right noexcept forward");
static_assert(noexcept(string_view{} <= evil_conversion_to_string_view_rvalue_only{}), "operator<= right noexcept forward");
static_assert(!noexcept(string_view{} <= convert_rvalue_only), "operator<= right noexcept forward");
static_assert(!noexcept(string_view{} <= evil_conversion_to_string_view_lvalue_only{}), "operator<= right noexcept forward");
static_assert(noexcept(string_view{} <= convert_lvalue_only), "operator<= right noexcept forward");

static_assert(noexcept(string_view{} >= string_view{}), "operator>= not noexcept");
static_assert(!noexcept(conversion_to_string_view{} >= string_view{}), "operator>= left noexcept forward");
static_assert(noexcept(evil_conversion_to_string_view_rvalue_only{} >= string_view{}), "operator>= left noexcept forward");
static_assert(!noexcept(convert_rvalue_only >= string_view{}), "operator>= left noexcept forward");
static_assert(!noexcept(evil_conversion_to_string_view_lvalue_only{} >= string_view{}), "operator>= left noexcept forward");
static_assert(noexcept(convert_lvalue_only >= string_view{}), "operator>= left noexcept forward");
static_assert(!noexcept(string_view{} >= conversion_to_string_view{}), "operator>= right noexcept forward");
static_assert(noexcept(string_view{} >= evil_conversion_to_string_view_rvalue_only{}), "operator>= right noexcept forward");
static_assert(!noexcept(string_view{} >= convert_rvalue_only), "operator>= right noexcept forward");
static_assert(!noexcept(string_view{} >= evil_conversion_to_string_view_lvalue_only{}), "operator>= right noexcept forward");
static_assert(noexcept(string_view{} >= convert_lvalue_only), "operator>= right noexcept forward");

template <typename CharT>
struct choose_literal; // not defined

template <>
struct choose_literal<char>
{
    static constexpr const char* choose(const char* s, const wchar_t*)
    {
        return s;
    }
};

template <>
struct choose_literal<wchar_t>
{
    static constexpr const wchar_t* choose(const char*, const wchar_t* s)
    {
        return s;
    }
};

#define TYPED_LITERAL(CharT, Literal) (choose_literal<CharT>::choose(Literal, L##Literal))

constexpr bool test_case_default_constructor()
{
    string_view sv;
    assert(sv.data() == nullptr);
    assert(sv.size() == 0);
    assert(sv.empty());

    return true;
}

template <typename Traits = char_traits<char>>
constexpr bool test_case_ntcts_constructor()
{
    const char* const expectedData = "null-terminated";
    basic_string_view<char, Traits> sv(expectedData);
    assert(sv.data() == expectedData);
    assert(sv.size() == 15);
    assert(sv.length() == 15);
    assert(!sv.empty());
    assert(sv[1] == 'u');
    assert(sv.at(1) == 'u');
    assert(sv.front() == 'n');
    assert(sv.back() == 'd');

    const char* const expectedEmptyData = "";
    basic_string_view<char, Traits> svEmpty(expectedEmptyData);
    assert(svEmpty.data() == expectedEmptyData);
    assert(svEmpty.size() == 0);
    assert(svEmpty.length() == 0);
    assert(svEmpty.empty());

    return true;
}

constexpr bool test_case_buffer_constructor()
{
    const char expectedDataArr[]   = {'n', 'o', ' ', 'n', 'u', 'l', 'l'};
    const char* const expectedData = expectedDataArr;
    string_view sv(expectedData, 7);
    assert(sv.data() == expectedData);
    assert(sv.size() == 7);
    assert(sv.length() == 7);
    assert(!sv.empty());
    assert(sv[1] == 'o');
    assert(sv.at(1) == 'o');
    assert(sv.front() == 'n');
    assert(sv.back() == 'l');

    return true;
}

constexpr bool test_case_contiguous_constructor()
{
    return true;
}

constexpr bool test_case_range_constructor()
{
    return true;
}

template <class CharT, class Traits>
constexpr bool test_case_iterators()
{
    using iterator = typename basic_string_view<CharT, Traits>::iterator;
    iterator itUninit{};
    iterator itCopy{itUninit};
    assert(itUninit == itCopy);

    basic_string_view<CharT, Traits> sv(TYPED_LITERAL(CharT, "hello world"));
    itUninit = sv.begin();
    itCopy   = itUninit;
    assert(itUninit == itCopy);
    assert(sv.end() - sv.begin() == 11);
    assert(sv.rend() - sv.rbegin() == 11);
    iterator testIterator = sv.begin() + 6;
    assert(*testIterator == static_cast<CharT>('w'));
    assert(*(testIterator.operator->()) == static_cast<CharT>('w'));
    ++testIterator;
    assert(*testIterator == static_cast<CharT>('o'));
    --testIterator;
    assert(*testIterator == static_cast<CharT>('w'));
    assert(*testIterator++ == static_cast<CharT>('w'));
    assert(*testIterator == static_cast<CharT>('o'));
    assert(*testIterator-- == static_cast<CharT>('o'));
    assert(*testIterator == static_cast<CharT>('w'));

    while (testIterator != sv.end())
    {
        ++testIterator;
    }
    while (testIterator != sv.begin())
    {
        --testIterator;
    }

    testIterator += 0;
    assert(*testIterator == static_cast<CharT>('h'));
    testIterator += 8;
    assert(*testIterator == static_cast<CharT>('r'));
    testIterator += -4;
    assert(*testIterator == static_cast<CharT>('o'));

    assert(*(testIterator + 6) == static_cast<CharT>('d'));
    assert(*(6 + testIterator) == static_cast<CharT>('d'));

    testIterator -= 1;
    assert(*testIterator == static_cast<CharT>('l'));

    assert(*(testIterator - 3) == static_cast<CharT>('h'));
    assert(testIterator[1] == static_cast<CharT>('o'));

    iterator eq    = testIterator;
    iterator after = testIterator + 2;

    assert(testIterator == testIterator);
    assert(testIterator == eq);
    assert(!(testIterator == after));
    assert(!(after == testIterator));

    assert(!(testIterator != testIterator));
    assert(!(testIterator != eq));
    assert(testIterator != after);
    assert(after != testIterator);

    assert(!(testIterator < testIterator));
    assert(!(testIterator < eq));
    assert(testIterator < after);
    assert(!(after < testIterator));

    assert(!(testIterator > testIterator));
    assert(!(testIterator > eq));
    assert(!(testIterator > after));
    assert(after > testIterator);

    assert(testIterator <= testIterator);
    assert(testIterator <= eq);
    assert(testIterator <= after);
    assert(!(after <= testIterator));

    assert(testIterator >= testIterator);
    assert(testIterator >= eq);
    assert(!(testIterator >= after));
    assert(after >= testIterator);

    using reverse_iterator = typename basic_string_view<CharT, Traits>::reverse_iterator;
    reverse_iterator reverseUninitIt{};
    reverse_iterator reverseUninitItCopy{reverseUninitIt};
    assert(reverseUninitIt == reverseUninitItCopy);
    assert(*sv.rbegin() == static_cast<CharT>('d'));
    assert(sv.rend()[-1] == static_cast<CharT>('h'));
    assert(sv.rend().base() == sv.begin());
    assert(sv.rbegin().base() == sv.end());
    reverse_iterator testRIterator = sv.rbegin() + 6;
    assert(*testRIterator == static_cast<CharT>('o'));
    testRIterator += 3;
    assert(*testRIterator == static_cast<CharT>('e'));
    testRIterator -= 9;
    assert(*testRIterator == static_cast<CharT>('d'));
    assert(*(testRIterator.operator->()) == static_cast<CharT>('d'));

    ++testRIterator;
    assert(*testRIterator == static_cast<CharT>('l'));
    assert(*testRIterator++ == static_cast<CharT>('l'));
    assert(*testRIterator == static_cast<CharT>('r'));
    --testRIterator;
    assert(*testRIterator == static_cast<CharT>('l'));
    assert(*testRIterator-- == static_cast<CharT>('l'));
    assert(*testRIterator == static_cast<CharT>('d'));

    reverse_iterator revEq    = testRIterator;
    reverse_iterator revAfter = testRIterator + 1;

    assert(testRIterator == testRIterator);
    assert(testRIterator == revEq);
    assert(!(testRIterator == revAfter));
    assert(!(revAfter == testRIterator));

    assert(!(testRIterator != testRIterator));
    assert(!(testRIterator != revEq));
    assert(testRIterator != revAfter);
    assert(revAfter != testRIterator);

    assert(!(testRIterator < testRIterator));
    assert(!(testRIterator < revEq));
    assert(testRIterator < revAfter);
    assert(!(revAfter < testRIterator));

    assert(!(testRIterator > testRIterator));
    assert(!(testRIterator > revEq));
    assert(!(testRIterator > revAfter));
    assert(revAfter > testRIterator);

    assert(testRIterator <= testRIterator);
    assert(testRIterator <= revEq);
    assert(testRIterator <= revAfter);
    assert(!(revAfter <= testRIterator));

    assert(testRIterator >= testRIterator);
    assert(testRIterator >= revEq);
    assert(!(testRIterator >= revAfter));
    assert(revAfter >= testRIterator);

    return true;
}

template <class CharT, class Traits>
constexpr bool test_case_prefix()
{
    basic_string_view<CharT, Traits> sv(TYPED_LITERAL(CharT, "hello world"));
    sv.remove_prefix(6);
    assert((sv == basic_string_view<CharT, Traits>{TYPED_LITERAL(CharT, "world")}));

    return true;
}

template <class CharT, class Traits>
constexpr bool test_case_suffix()
{
    basic_string_view<CharT, Traits> sv(TYPED_LITERAL(CharT, "hello world"));
    sv.remove_suffix(6);
    assert((sv == basic_string_view<CharT, Traits>{TYPED_LITERAL(CharT, "hello")}));

    return true;
}

template <class CharT, class Traits>
constexpr bool test_case_swap()
{
    basic_string_view<CharT, Traits> a(TYPED_LITERAL(CharT, "a"));
    basic_string_view<CharT, Traits> b(TYPED_LITERAL(CharT, "b"));
    a.swap(b);
    assert(a == TYPED_LITERAL(CharT, "b"));
    assert(b == TYPED_LITERAL(CharT, "a"));

    return true;
}

void test_case_at_out_of_range()
{
    string_view sv("text");
    // assert_throws<out_of_range>([&] { return sv.at(4); });
}

using copy_buff_ty = char[5];
constexpr void fill_exclaim(copy_buff_ty& a)
{
    for (size_t idx = 0; idx < 5; ++idx)
    {
        a[idx] = '!';
    }
}

constexpr bool copy_buff_eq(const copy_buff_ty& lhs, const copy_buff_ty& rhs)
{
    for (size_t idx = 0; idx < 5; ++idx)
    {
        if (lhs[idx] != rhs[idx])
        {
            return false;
        }
    }

    return true;
}

template <class Traits>
constexpr bool test_case_copy()
{
    basic_string_view<char, Traits> sv("text");
    copy_buff_ty buffer{};
    fill_exclaim(buffer);
    sv.copy(buffer, 5);
    assert(copy_buff_eq(buffer, {'t', 'e', 'x', 't', '!'}));
    fill_exclaim(buffer);
    sv.copy(buffer, 5, 2);
    assert(copy_buff_eq(buffer, {'x', 't', '!', '!', '!'}));
    fill_exclaim(buffer);
    sv.copy(buffer, 2);
    assert(copy_buff_eq(buffer, {'t', 'e', '!', '!', '!'}));
    fill_exclaim(buffer);
    sv.copy(buffer, 1, 2);
    assert(copy_buff_eq(buffer, {'x', '!', '!', '!', '!'}));

    return true;
}

void test_case_copy_out_of_range()
{
    string_view sv("text");
    copy_buff_ty buffer;
    // assert_throws<out_of_range>([&] { sv.copy(buffer, 4, 10); });
}

template <class Traits>
constexpr bool test_case_substr()
{
    basic_string_view<char, Traits> sv("text");

    assert(sv.substr() == sv);
    assert((sv.substr(2) == basic_string_view<char, Traits>{"xt"}));
    assert((sv.substr(0, 2) == basic_string_view<char, Traits>{"te"}));
    assert((sv.substr(1, 2) == basic_string_view<char, Traits>{"ex"}));

    return true;
}

void test_case_substr_out_of_range()
{
    // assert_throws<out_of_range>([] { return string_view("text").substr(5); });
}

template <class CharT, class Traits>
constexpr bool test_case_compare()
{
    const auto npos = basic_string_view<CharT, Traits>::npos;
    basic_string_view<CharT, Traits> rr(TYPED_LITERAL(CharT, "rr"));
    basic_string_view<CharT, Traits> s4(TYPED_LITERAL(CharT, "s4"));
    basic_string_view<CharT, Traits> s4s4(TYPED_LITERAL(CharT, "s4s4"));

    assert(rr.compare(rr) == 0); // equal

    assert(rr.compare(s4) < 0); // differ by contents only
    assert(s4.compare(rr) > 0);

    assert(s4.compare(s4s4) < 0); // differ by length only
    assert(s4s4.compare(s4) > 0);

    assert(rr.compare(1, 0, basic_string_view<CharT, Traits>{}) == 0);
    assert(rr.compare(0, 1000, rr) == 0);
    assert(s4s4.compare(2, 2, s4) == 0);
    assert(s4s4.compare(2, 200, s4) == 0);

    assert(rr.compare(0, npos, s4s4, 0, 2) < 0);
    assert(s4.compare(0, npos, s4s4, 2, 2) == 0);
    assert(s4.compare(0, npos, s4s4, 0, 2) == 0);
    assert(s4.compare(0, npos, s4s4, 2, 1000) == 0);

    assert(rr.compare(TYPED_LITERAL(CharT, "rr")) == 0);
    assert(rr.compare(TYPED_LITERAL(CharT, "s4")) < 0);
    assert(rr.compare(TYPED_LITERAL(CharT, "s4s4")) < 0);
    assert(s4.compare(TYPED_LITERAL(CharT, "s4")) == 0);
    assert(s4.compare(TYPED_LITERAL(CharT, "rr")) > 0);

    assert(rr.compare(1, 0, TYPED_LITERAL(CharT, "")) == 0);
    assert(rr.compare(1, 0, TYPED_LITERAL(CharT, "rr")) < 0);
    assert(rr.compare(0, 1000, TYPED_LITERAL(CharT, "rr")) == 0);
    assert(s4.compare(0, 2, TYPED_LITERAL(CharT, "s4s4")) < 0);
    assert(s4s4.compare(2, 2, TYPED_LITERAL(CharT, "s4")) == 0);
    assert(s4s4.compare(2, 200, TYPED_LITERAL(CharT, "s4")) == 0);
    assert(s4s4.compare(1, 2, TYPED_LITERAL(CharT, "s4")) < 0);

    return true;
}

void test_case_compare_out_of_range()
{
    // assert_throws<out_of_range>([] { return string_view{"hi"}.compare(10, 2, string_view{}); });
    // assert_throws<out_of_range>([] { return string_view{"hi"}.compare(10, 2, string_view{"other"}, 4, 2); });
    // assert_throws<out_of_range>([] { return string_view{"hi"}.compare(0, 2, string_view{"other"}, 6, 2); });
    // assert_throws<out_of_range>([] { return string_view{"hi"}.compare(3, 0, "hello"); });
    // assert_throws<out_of_range>([] { return string_view{"hi"}.compare(3, 0, "hello", 5); });
}

template <class CharT, class Traits, bool TestBasicString>
constexpr bool test_case_starts_with_ends_with()
{
    const basic_string_view<CharT, Traits> empty_sv(TYPED_LITERAL(CharT, ""));
    const CharT null_c = '\0';
    const CharT* const empty_cp(TYPED_LITERAL(CharT, ""));

    const basic_string_view<CharT, Traits> a(TYPED_LITERAL(CharT, "a"));
    const CharT b = 'b';
    const CharT* const c(TYPED_LITERAL(CharT, "c"));

    const basic_string_view<CharT, Traits> moving_sv(TYPED_LITERAL(CharT, "moving"));
    const CharT* const moving_cp(TYPED_LITERAL(CharT, "moving"));

    if constexpr (TestBasicString)
    {
        basic_string<CharT, Traits> text(TYPED_LITERAL(CharT, "text"));
        basic_string<CharT, Traits> empty_text(TYPED_LITERAL(CharT, ""));

        const basic_string_view<CharT, Traits> te(TYPED_LITERAL(CharT, "te"));
        const CharT t = 't';
        const CharT* const tex(TYPED_LITERAL(CharT, "tex"));
        const basic_string_view<CharT, Traits> text_sv(TYPED_LITERAL(CharT, "text"));
        const CharT* const text_cp(TYPED_LITERAL(CharT, "text"));
        const basic_string_view<CharT, Traits> next_sv(TYPED_LITERAL(CharT, "next"));
        const CharT* const next_cp(TYPED_LITERAL(CharT, "next"));
        const basic_string_view<CharT, Traits> texture_sv(TYPED_LITERAL(CharT, "texture"));
        const CharT* const texture_cp(TYPED_LITERAL(CharT, "texture"));

        const basic_string_view<CharT, Traits> xt(TYPED_LITERAL(CharT, "xt"));
        const CharT* const ext(TYPED_LITERAL(CharT, "ext"));
        const basic_string_view<CharT, Traits> context_sv(TYPED_LITERAL(CharT, "context"));
        const CharT* const context_cp(TYPED_LITERAL(CharT, "context"));
    }

    const basic_string_view<CharT, Traits> hello(TYPED_LITERAL(CharT, "hello"));

    const basic_string_view<CharT, Traits> he(TYPED_LITERAL(CharT, "he"));
    const CharT h = 'h';
    const CharT* const hel(TYPED_LITERAL(CharT, "hel"));
    const basic_string_view<CharT, Traits> hello_sv(TYPED_LITERAL(CharT, "hello"));
    const CharT* const hello_cp(TYPED_LITERAL(CharT, "hello"));
    const basic_string_view<CharT, Traits> cello_sv(TYPED_LITERAL(CharT, "cello"));
    const CharT* const cello_cp(TYPED_LITERAL(CharT, "cello"));
    const basic_string_view<CharT, Traits> helloworld_sv(TYPED_LITERAL(CharT, "helloworld"));
    const CharT* const helloworld_cp(TYPED_LITERAL(CharT, "helloworld"));

    assert(hello.starts_with(he));
    assert(hello.starts_with(h));
    assert(hello.starts_with(hel));
    assert(hello.starts_with(hello));
    assert(hello.starts_with(hello_sv));
    assert(hello.starts_with(hello_cp));
    assert(!hello.starts_with(cello_sv));
    assert(!hello.starts_with(cello_cp));
    assert(!hello.starts_with(helloworld_sv));
    assert(!hello.starts_with(helloworld_cp));
    assert(hello.starts_with(empty_sv));
    assert(!hello.starts_with(null_c));
    assert(hello.starts_with(empty_cp));
    assert(!hello.starts_with(a));
    assert(!hello.starts_with(b));
    assert(!hello.starts_with(c));

    assert(!empty_sv.starts_with(he));
    assert(!empty_sv.starts_with(h));
    assert(!empty_sv.starts_with(hel));
    assert(empty_sv.starts_with(empty_sv));
    assert(!empty_sv.starts_with(null_c));
    assert(empty_sv.starts_with(empty_cp));

    const basic_string_view<CharT, Traits> lo(TYPED_LITERAL(CharT, "lo"));
    const CharT o = 'o';
    const CharT* const llo(TYPED_LITERAL(CharT, "llo"));
    const basic_string_view<CharT, Traits> worldhello_sv(TYPED_LITERAL(CharT, "worldhello"));
    const CharT* const worldhello_cp(TYPED_LITERAL(CharT, "worldhello"));

    assert(hello.ends_with(lo));
    assert(hello.ends_with(o));
    assert(hello.ends_with(llo));
    assert(hello.ends_with(hello));
    assert(hello.ends_with(hello_sv));
    assert(hello.ends_with(hello_cp));
    assert(!hello.ends_with(cello_sv));
    assert(!hello.ends_with(cello_cp));
    assert(!hello.ends_with(worldhello_sv));
    assert(!hello.ends_with(worldhello_cp));
    assert(hello.ends_with(empty_sv));
    assert(!hello.ends_with(null_c));
    assert(hello.ends_with(empty_cp));
    assert(!hello.ends_with(a));
    assert(!hello.ends_with(b));
    assert(!hello.ends_with(c));

    assert(!empty_sv.ends_with(lo));
    assert(!empty_sv.ends_with(o));
    assert(!empty_sv.ends_with(llo));
    assert(empty_sv.ends_with(empty_sv));
    assert(!empty_sv.ends_with(null_c));
    assert(empty_sv.ends_with(empty_cp));

    const basic_string_view<CharT, Traits> playing(TYPED_LITERAL(CharT, "playing"));

    const basic_string_view<CharT, Traits> player_sv(TYPED_LITERAL(CharT, "player"));
    const CharT* const player_cp(TYPED_LITERAL(CharT, "player"));

    assert(!playing.starts_with(player_sv));
    assert(!playing.starts_with(player_cp));

    assert(!playing.ends_with(moving_sv));
    assert(!playing.ends_with(moving_cp));

    return true;
}

template <typename CharT, typename Traits>
constexpr bool test_case_find()
{
    basic_string_view<CharT, Traits> aaa(TYPED_LITERAL(CharT, "aaa"));
    basic_string_view<CharT, Traits> s4(TYPED_LITERAL(CharT, "s4"));
    basic_string_view<CharT, Traits> s4s4(TYPED_LITERAL(CharT, "s4s4"));
    basic_string_view<CharT, Traits> sssss4(TYPED_LITERAL(CharT, "sssss4"));
    basic_string_view<CharT, Traits> s5(TYPED_LITERAL(CharT, "s5"));

    basic_string_view<CharT, Traits> embeddedNullsHaystack(TYPED_LITERAL(CharT, "za\0bz"), 5);
    basic_string_view<CharT, Traits> embeddedNullsNeedle(TYPED_LITERAL(CharT, "a\0b"), 3);

    const auto npos = basic_string_view<CharT, Traits>::npos;

    assert(s4s4.find(s4) == 0);
    assert(s4s4.find(s4, 1) == 2);
    assert(s4s4.find(s4, 3) == npos);
    assert(s4s4.find(TYPED_LITERAL(CharT, "s4")) == 0);
    assert(s4s4.find(TYPED_LITERAL(CharT, "s4"), 3) == npos); // _Start_at > _Size - _Count
    assert(s4s4.find(TYPED_LITERAL(CharT, "s4XX"), 1, 2) == 2);
    assert(s4s4.find(TYPED_LITERAL(CharT, ""), 2) == 2);    // empty string always matches
    assert(s4s4.find(TYPED_LITERAL(CharT, ""), 4) == 4);    // even at the end
    assert(s4s4.find(TYPED_LITERAL(CharT, ""), 5) == npos); // but not off the end
    assert(s4.find(TYPED_LITERAL(CharT, "s4XX")) == npos);  // _Count > _Size
    assert(s4s4.find(static_cast<CharT>('s')) == 0);
    assert(s4s4.find(static_cast<CharT>('s'), 1) == 2);
    assert(s4s4.find(static_cast<CharT>('x')) == npos);
    assert(sssss4.find(s4) == 4);
    assert(embeddedNullsHaystack.find(embeddedNullsNeedle) == 1);

    assert(s4s4.rfind(s4) == 2);
    assert(s4s4.rfind(s4, 1) == 0);
    assert(s4s4.rfind(s5, 3) == npos);
    assert(s4s4.rfind(TYPED_LITERAL(CharT, "s4")) == 2);
    assert(s4s4.rfind(TYPED_LITERAL(CharT, "s4"), 3) == 2);
    assert(s4s4.rfind(TYPED_LITERAL(CharT, "s4XX"), 1, 3) == npos);
    assert(s4s4.rfind(static_cast<CharT>('s')) == 2);
    assert(s4s4.rfind(static_cast<CharT>('s'), 2) == 2);
    assert(s4s4.rfind(static_cast<CharT>('x')) == npos);

    assert(s4s4.find_first_of(s4) == 0);
    assert(s4s4.find_first_of(s4, 1) == 1);
    assert(s4s4.find_first_of(s4, 4) == npos);
    assert(s4s4.find_first_of(TYPED_LITERAL(CharT, "s4")) == 0);
    assert(s4s4.find_first_of(TYPED_LITERAL(CharT, "s4"), 3) == 3);
    assert(s4s4.find_first_of(TYPED_LITERAL(CharT, "abs"), 1, 2) == npos);
    assert(s4s4.find_first_of(static_cast<CharT>('s')) == 0);
    assert(s4s4.find_first_of(static_cast<CharT>('s'), 1) == 2);
    assert(s4s4.find_first_of(static_cast<CharT>('x')) == npos);

    assert(s4s4.find_last_of(s4) == 3);
    assert(s4s4.find_last_of(s4, 1) == 1);
    assert(s4s4.find_last_of(aaa) == npos);
    assert(s4s4.find_last_of(TYPED_LITERAL(CharT, "s4")) == 3);
    assert(s4s4.find_last_of(TYPED_LITERAL(CharT, "s4"), 2) == 2);
    assert(s4s4.find_last_of(TYPED_LITERAL(CharT, "abs"), 1, 2) == npos);
    assert(s4s4.find_last_of(static_cast<CharT>('s')) == 2);
    assert(s4s4.find_last_of(static_cast<CharT>('s'), 1) == 0);
    assert(s4s4.find_last_of(static_cast<CharT>('x')) == npos);

    assert(s4s4.find_first_not_of(s5) == 1);
    assert(s4s4.find_first_not_of(s5, 2) == 3);
    assert(s4s4.find_first_not_of(s4) == npos);
    assert(s4s4.find_first_not_of(TYPED_LITERAL(CharT, "s5")) == 1);
    assert(s4s4.find_first_not_of(TYPED_LITERAL(CharT, "s5"), 2) == 3);
    assert(s4s4.find_first_not_of(TYPED_LITERAL(CharT, "s4a"), 1, 2) == npos);
    assert(s4s4.find_first_not_of(static_cast<CharT>('s')) == 1);
    assert(s4s4.find_first_not_of(static_cast<CharT>('s'), 2) == 3);
    assert(s4s4.find_first_not_of(static_cast<CharT>('s'), 4) == npos);

    assert(s4s4.find_last_not_of(s5) == 3);
    assert(s4s4.find_last_not_of(s5, 2) == 1);
    assert(s4s4.find_last_not_of(s4) == npos);
    assert(s4s4.find_last_not_of(TYPED_LITERAL(CharT, "s5")) == 3);
    assert(s4s4.find_last_not_of(TYPED_LITERAL(CharT, "s5"), 2) == 1);
    assert(s4s4.find_last_not_of(TYPED_LITERAL(CharT, "s4a"), 1, 2) == npos);
    assert(s4s4.find_last_not_of(static_cast<CharT>('s')) == 3);
    assert(s4s4.find_last_not_of(static_cast<CharT>('s'), 2) == 1);
    assert(s4s4.find_last_not_of(static_cast<CharT>('s'), 0) == npos);

    return true;
}

template <class CharT, class Traits>
constexpr bool test_case_operators()
{
    basic_string_view<CharT, Traits> rr(TYPED_LITERAL(CharT, "rr"));
    basic_string_view<CharT, Traits> s4(TYPED_LITERAL(CharT, "s4"));
    basic_string_view<CharT, Traits> cpy(s4);

    assert(s4 == cpy);
    assert(cpy == s4);
    assert(!(s4 == rr));
    assert(!(rr == s4));
    assert(TYPED_LITERAL(CharT, "s4") == s4); // converting operator left
    assert(!(TYPED_LITERAL(CharT, "rr") == s4));
    assert(s4 == TYPED_LITERAL(CharT, "s4")); // converting operator right
    assert(!(s4 == TYPED_LITERAL(CharT, "s4s4")));

    assert(!(s4 != cpy));
    assert(!(cpy != s4));
    assert(s4 != rr);
    assert(rr != s4);
    assert(!(TYPED_LITERAL(CharT, "s4") != s4));
    assert(TYPED_LITERAL(CharT, "rr") != s4);
    assert(!(s4 != TYPED_LITERAL(CharT, "s4")));
    assert(s4 != TYPED_LITERAL(CharT, "s4s4"));

    assert(!(s4 < cpy));
    assert(!(cpy < s4));
    assert(!(s4 < rr));
    assert(rr < s4);
    assert(!(TYPED_LITERAL(CharT, "s4") < s4));
    assert(!(s4 < TYPED_LITERAL(CharT, "s4")));
    assert(s4 < TYPED_LITERAL(CharT, "s4s4"));

    assert(!(s4 > cpy));
    assert(!(cpy > s4));
    assert(s4 > rr);
    assert(!(rr > s4));
    assert(!(TYPED_LITERAL(CharT, "s4") > s4));
    assert(!(s4 > TYPED_LITERAL(CharT, "s4")));
    assert(!(s4 > TYPED_LITERAL(CharT, "s4s4")));

    assert(s4 <= cpy);
    assert(cpy <= s4);
    assert(!(s4 <= rr));
    assert(rr <= s4);
    assert(TYPED_LITERAL(CharT, "s4") <= s4);
    assert(s4 <= TYPED_LITERAL(CharT, "s4"));
    assert(s4 <= TYPED_LITERAL(CharT, "s4s4"));

    assert(s4 >= cpy);
    assert(cpy >= s4);
    assert(s4 >= rr);
    assert(!(rr >= s4));
    assert(TYPED_LITERAL(CharT, "s4") >= s4);
    assert(s4 >= TYPED_LITERAL(CharT, "s4"));
    assert(!(s4 >= TYPED_LITERAL(CharT, "s4s4")));

    return true;
}

void test_case_inserter()
{
    ostringstream oss;
    string_view sv("text");
    oss << sv;
    assert(oss.str() == "text");
}

void test_case_hashing()
{
    string str("hungry EVIL zombies");
    string_view strView(str);
    assert(hash<string>{}(str) == hash<string_view>{}(strView));
}

// Also test that no C6510 warning
struct char_wrapper
{
    char c;
};

template <>
struct std::char_traits<char_wrapper>
{
    using char_type = char_wrapper;

    static bool eq(char_wrapper lhs, char_wrapper rhs)
    {
        return lhs.c == rhs.c;
    }

    static size_t length(const char_wrapper* a)
    {
        static_assert(sizeof(char_wrapper) == 1, "strlen requires this");
        return strlen(reinterpret_cast<const char*>(a));
    }

    static int compare(const char_wrapper* lhs, const char_wrapper* rhs, size_t count)
    {
        return char_traits<char>::compare(
            reinterpret_cast<const char*>(lhs), reinterpret_cast<const char*>(rhs), count);
    }
};

using WrappedSV = basic_string_view<char_wrapper, char_traits<char_wrapper>>;

void test_C6510_warning()
{
    char_wrapper a[] = {{'a'}, {'b'}, {'c'}, {'\0'}};
    WrappedSV sv(a);
    (void)sv;
}

TEST(string_view_test, test2)
{
    test_case_default_constructor();
    test_case_ntcts_constructor();
    test_case_buffer_constructor();
    test_case_contiguous_constructor();
    test_case_range_constructor();
    test_case_iterators<char, char_traits<char>>();
    test_case_iterators<wchar_t, char_traits<wchar_t>>();
    test_case_prefix<char, char_traits<char>>();
    test_case_prefix<wchar_t, char_traits<wchar_t>>();
    test_case_suffix<char, char_traits<char>>();
    test_case_suffix<wchar_t, char_traits<wchar_t>>();
    test_case_swap<char, char_traits<char>>();
    test_case_swap<wchar_t, char_traits<wchar_t>>();
    test_case_at_out_of_range();
    test_case_copy<char_traits<char>>();
    test_case_copy_out_of_range();
    test_case_substr<char_traits<char>>();
    test_case_substr_out_of_range();
    test_case_compare<char, char_traits<char>>();
    test_case_compare<wchar_t, char_traits<wchar_t>>();
    test_case_compare_out_of_range();
    test_case_starts_with_ends_with<char, char_traits<char>, true>();
    test_case_starts_with_ends_with<wchar_t, char_traits<wchar_t>, true>();
    test_case_find<char, char_traits<char>>();
    test_case_find<wchar_t, char_traits<wchar_t>>();
    test_case_operators<char, char_traits<char>>();
    test_case_operators<wchar_t, char_traits<wchar_t>>();
    test_case_inserter();
    test_case_hashing();
}