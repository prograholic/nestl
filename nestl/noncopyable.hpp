#ifndef NESTL_NONCOPYABLE_HPP
#define NESTL_NONCOPYABLE_HPP

#include <nestl/config.hpp>

/**
 * @file implementation of noncopyable class (taken from Boost)
 */

namespace nestl
{

namespace noncopyable_  // protection from unintended ADL
{

class noncopyable
{
protected:
    NESTL_CONSTEXPR noncopyable() NESTL_COMPILER_GENERATED_IMPL;
    ~noncopyable() NESTL_COMPILER_GENERATED_IMPL;

    noncopyable( const noncopyable& ) NESTL_DELETED_IMPL;
    noncopyable& operator=( const noncopyable& ) NESTL_DELETED_IMPL;
};

} // namespace noncopyable_

typedef noncopyable_::noncopyable noncopyable;

} // namespace nestl

#endif // NESTL_NONCOPYABLE_HPP
