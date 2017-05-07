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
    NESTL_CONSTEXPR noncopyable() = default;
    ~noncopyable() = default;

    noncopyable( const noncopyable& ) = delete;
    noncopyable& operator=( const noncopyable& ) = delete;
};

} // namespace noncopyable_

typedef noncopyable_::noncopyable noncopyable;

} // namespace nestl

#endif // NESTL_NONCOPYABLE_HPP
