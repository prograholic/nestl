#ifndef NESTL_TYPE_TRAITS_H
#define NESTL_TYPE_TRAITS_H

#include <nestl/config.hpp>

#include <type_traits>

namespace nestl
{


// NOTE: we use template struct instead of `using` as MSVC-2015 does, because `using` does not work in case of SFINAE error
template<typename ...>
struct void_t
{
    typedef void type;
};


/// @brief This template describes ability of two-phase initialization of given type T
///
/// Client may allow two-phase initialization by specialization this struct with concrete type T
/// Also such specialization should provide static method init with following signature:
/// @code
/// template <typename OperationError>
/// static void init(OperationError& err, T& defaultConstructed, Args&& ... args) NESTL_NOEXCEPT_SPEC
/// @endcode
///
/// @note There may be several static methods `init` in one specialization
///
/// Please note that object of T already default constructed.
/// Also no need to perform destruction of defaultConstructed in case of failure
template <typename T>
struct two_phase_initializator
{
};

} // namespace nestl

#endif /* NESTL_TYPE_TRAITS_H */
