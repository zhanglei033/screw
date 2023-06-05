#pragma once
#ifndef _ASSERT_H_
#define _ASSERT_H_
#include "boost/preprocessor/stringize.hpp"
#include <stdexcept>

#define ThrowException(expr, message)                                                                                                  \
    if (!(expr))                                                                                                                       \
    {                                                                                                                                  \
        throw std::logic_error(std::string("\"" #expr "\""                                                                           \
                                             "file:" BOOST_PP_STRINGIZE(__FILE__) ", line " BOOST_PP_STRINGIZE(__LINE__) ",message: ") \
                                             + message);                                                                               \
    }

#ifndef NDEBUG
#define ASSERT(expr, message)                                                                                          \
    if (!(expr))                                                                                                       \
    {                                                                                                                  \
        ::fputs((std::string("\"" #expr "\""                                                                           \
                             "file:" BOOST_PP_STRINGIZE(__FILE__) ", line " BOOST_PP_STRINGIZE(__LINE__) ",message: ") \
                             + message)                                                                                \
                                 .data(),                                                                              \
                 stderr);                                                                                              \
        ::abort();                                                                                                     \
    }
#else
#define ASSERT(expr, message)
#endif // !NDEBUG

#endif // !_ASSERT_H_
