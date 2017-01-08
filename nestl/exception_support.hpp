#ifndef NESTL_EXCEPTION_SUPPORT_HPP
#define NESTL_EXCEPTION_SUPPORT_HPP

#include <nestl/config.hpp>
#include <nestl/type_traits.hpp>

namespace nestl
{
    struct global_exception_support : integral_constant<bool, NESTL_HAS_EXCEPTIONS>
    {
    };
    
    /// @note client may add specialization of this structure for custom types
    /// @note Actually client may only forbide usage of T in exception operations, because when exceptions are disabled this does not make sense at all
    template <typename T>
    struct can_deal_with_exceptions : global_exception_support
    {
    };
    
    /// @note client may add specialization of this structure for custom types
    /// @todo write default checker which checks for particular methods constructors and operators
    template <typename T>
    struct can_deal_with_noexcept : true_type
    {
    };
    
    
    /// @brief this template is intended for checking whether given type can be used with exceptions or not
    template <typename T>
    struct exception_support_checker : integral_constant<bool, global_exception_support::value && can_deal_with_exceptions<T>::value>
    {
    };
    
    /// @brief this template is intended for checking whether given type can be used with noexcept operations or not
    template <typename T>
    struct noexcept_support_checker : can_deal_with_noexcept<T>
    {
    };

} // namespace nestl

#endif /* NESTL_EXCEPTION_SUPPORT_HPP */