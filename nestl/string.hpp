#ifndef NESTL_STRING_HPP
#define NESTL_STRING_HPP

#include <nestl/config.hpp>

#include <nestl/char_traits.hpp>
#include <nestl/allocator.hpp>

namespace nestl
{

template<typename CharType, typename Traits, typename Alloc>
class basic_string
{

};


typedef basic_string<char, nestl::char_traits<char>, nestl::allocator<char> > string;





} // namespace nestl


#endif /* NESTL_STRING_HPP */
