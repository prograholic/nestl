/**
 * @file This file provides macros for selecting types and methods
 */


#ifndef NESTL_DETAIL_SELECT_TYPE_HPP
#define NESTL_DETAIL_SELECT_TYPE_HPP

#include <nestl/config.hpp>

namespace nestl
{

/**
 * @note This macro is taken from stdlibc++
 */
#   define NESTL_SELECT_NESTED_TYPE(Type, Nested, Default) \
    template<typename T> \
    static typename T::Nested NESTL_##Nested##_helper(T*); \
    static Default            NESTL_##Nested##_helper(...); \
    typedef decltype(NESTL_##Nested##_helper((Type*)0)) nestl_nested_type_##Nested \


#   define NESTL_SELECT_NESTED_TYPE_TEMPLATE(Type, NestedType, ResultType, Default) \
    template<typename T> \
    static typename T::template NestedType NESTL_##ResultType##_helper(T*); \
    static Default                         NESTL_##ResultType##_helper(...); \
    typedef decltype(NESTL_##ResultType##_helper((Type*)0)) nestl_nested_type_##ResultType \


#define NESTL_CHECK_METHOD_WITH_SIGNATURE(method) \
template<typename TypeWithSignatureOrNot, typename Signature> \
struct has_ ## method ## _impl \
{ \
    template <typename U, U> struct type_check; \
    template <typename TypeWithSignatureOrNot2> \
    static char test(type_check<Signature, &TypeWithSignatureOrNot2::method>*); \
    template <typename> \
    static long test(...); \
    enum \
    { \
        value = sizeof(test<TypeWithSignatureOrNot>(0)) == sizeof(char) \
    }; \
} \

}


#endif /* NESTL_DETAIL_SELECT_TYPE_HPP */
