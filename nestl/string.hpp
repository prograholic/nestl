#ifndef NESTL_STRING_HPP
#define NESTL_STRING_HPP

#include <nestl/config.hpp>

#include <nestl/exception_support.hpp>

#include <nestl/has_exceptions/string.hpp>
#include <nestl/no_exceptions/string.hpp>

namespace nestl
{

template<typename CharType, typename Traits, typename Alloc>
using basic_string = exception_support::dispatch<has_exceptions::basic_string<CharType, Traits, Alloc>,
                                                 no_exceptions::basic_string<CharType, Traits, Alloc>>;


typedef basic_string<char, std::char_traits<char>, nestl::allocator<char>> string;
typedef basic_string<wchar_t, std::char_traits<wchar_t>, nestl::allocator<wchar_t>> wstring;

} // namespace nestl

#endif /* NESTL_STRING_HPP */
