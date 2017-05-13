#ifndef NESTL_TYPE_TRAITS_H
#define NESTL_TYPE_TRAITS_H

#include <nestl/config.hpp>

#include <type_traits>

namespace nestl
{


/// @brief Trait describes ability of two-phase initialization of given type T with given arguments Args
///
/// @note By default we forbide two-phase initialization therefore this trait is a std::false_type
///
/// Client may allow two-phas initialization by specialization this struct with concrete type T (Args may be variadic template).
/// Also such specialization should provide static method init with following signature:
/// @code
/// template <typename OperationError>
/// static void init(OperationError& err, T& defaultConstructed, Args&& ... args) NESTL_NOEXCEPT_SPEC
/// @endcode
///
/// Please not that object of T already default constructed.
/// Also no need to perform destruction of defaultConstructed in case of failure
template <typename T, typename ... Args>
struct two_phase_initializable : std::false_type
{
};

} // namespace nestl

#endif /* NESTL_TYPE_TRAITS_H */
