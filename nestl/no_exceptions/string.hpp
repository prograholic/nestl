#ifndef NESTL_NO_EXCEPTIONS_STRING_HPP
#define NESTL_NO_EXCEPTIONS_STRING_HPP

#include <nestl/config.hpp>

#include <nestl/implementation/string.hpp>

namespace nestl
{
namespace no_exceptions
{


template<typename CharType, typename Traits, typename Alloc>
using basic_string = impl::basic_string<CharType, Traits, Alloc>;


typedef basic_string<char, std::char_traits<char>, nestl::allocator<char>> string;
typedef basic_string<wchar_t, std::char_traits<wchar_t>, nestl::allocator<wchar_t>> wstring;




} // namespace has_exceptions
} // namespace nestl
#endif /* NESTL_NO_EXCEPTIONS_STRING_HPP */
