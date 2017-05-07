/**
 * @file This file provides macros for selecting types and methods
 */


#ifndef NESTL_DETAIL_SELECT_TYPE_HPP
#define NESTL_DETAIL_SELECT_TYPE_HPP

#include <nestl/config.hpp>

namespace nestl
{

#if (NESTL_COMPILER == NESTL_COMPILER_GCC) || (NESTL_COMPILER == NESTL_COMPILER_CLANG)

/**
 * @note This macro is taken from stdlibc++
 */
#   define NESTL_SELECT_NESTED_TYPE(Type, Nested, Default) \
    template<typename T> \
    static typename T::Nested NESTL_##Nested##_helper(T*); \
    static Default            NESTL_##Nested##_helper(...); \
    typedef NESTL_DECLTYPE(NESTL_##Nested##_helper((Type*)0)) nestl_nested_type_##Nested \


#   define NESTL_SELECT_NESTED_TYPE_TEMPLATE(Type, NestedType, ResultType, Default) \
    template<typename T> \
    static typename T::template NestedType NESTL_##ResultType##_helper(T*); \
    static Default                         NESTL_##ResultType##_helper(...); \
    typedef NESTL_DECLTYPE(NESTL_##ResultType##_helper((Type*)0)) nestl_nested_type_##ResultType \



#elif NESTL_COMPILER == NESTL_COMPILER_MSVC

#   if defined(_MSC_EXTENSIONS)

#       define NESTL_SELECT_NESTED_TYPE(Type, Nested, Default) \
    __if_exists(Type::Nested) \
    { \
        typedef typename Type::Nested nestl_nested_type_ ## Nested; \
    }; \
    __if_not_exists(Type::Nested) \
    { \
        typedef Default nestl_nested_type_ ## Nested; \
    } \

#       define NESTL_SELECT_NESTED_TYPE_TEMPLATE(Type, NestedType, ResultType, Default) typedef Default nestl_nested_type_ ## ResultType

/// @bug msvc-2008 does not compile following type selector (maybe there is my bug)
#if 0
    __if_exists(Type::template NestedType) \
    { \
        typedef typename Type::template NestedType nestl_nested_type_ ## ResultType; \
    }; \
    __if_not_exists(Type::template NestedType) \
    { \
        typedef Default nestl_nested_type_ ## ResultType; \
    } \

#endif //0


#else /* _MSC_EXTENSIONS */

#   error "MSVC cannot select type without MS-specific extensions, please recompile with /Za"

#endif /* _MSC_EXTENSIONS */

#else /* NESTL_COMPILER */
#   error "Unsupported compiler"
#endif /* NESTL_COMPILER */



#define NESTL_CHECK_METHOD_WITH_SIGNATURE(Type, method) \
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
