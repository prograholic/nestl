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
    static Default NESTL_##Nested##_helper(...); \
    typedef NESTL_DECLTYPE(NESTL_##Nested##_helper((Type*)0)) nestl_nested_type_##Nested \


#   define NESTL_SELECT_NESTED_TYPE_TEMPLATE(Type, NestedType, ResultType, Default) \
    template<typename T> \
    static typename T::template NestedType NESTL_##ResultType##_helper(T*); \
    static Default NESTL_##ResultType##helper(...); \
    typedef NESTL_DECLTYPE(NESTL_##ResultType##_helper((Type*)0)) nestl_nested_type_##ResultType \



#   define NESTL_HAS_METHOD(Class, methodName) \
    template <typename T> \
    class has_ ## methodName ## _member_impl \
    { \
        typedef char has_method; \
        typedef long does_not_has_method; \
        NESTL_STATIC_ASSERT(sizeof(has_method) != sizeof(does_not_has_method), \
                            "please, select types with different size"); \
        template <typename C> static has_method test( NESTL_DECLTYPE(nestl::declval(&C::methodName)) ) ; \
        template <typename C> static does_not_has_method test(...); \
    public: \
        typedef typename nestl::conditional \
        < \
            sizeof(test<T>(0)) == sizeof(has_method), \
            nestl::true_type, \
            nestl::false_type \
        >::type type; \
    }; \
    typedef typename has_ ## methodName ## _member_impl<Class>::type has_ ## methodName ## _member \


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


#       define NESTL_HAS_METHOD(Class, methodName) \
    __if_exists(Class::methodName) \
    { \
        typedef nestl::true_type has_ ## methodName ## _member; \
    }; \
    __if_not_exists(Class::methodName) \
    { \
        typedef nestl::false_type has_ ## methodName ## _member; \
    } \

#else /* _MSC_EXTENSIONS */

#   error "MSVC cannot select type without MS-specific extensions, please recompile with /Za"

#endif /* _MSC_EXTENSIONS */

#else /* NESTL_COMPILER */
#   error "Unsupported compiler"
#endif /* NESTL_COMPILER */

}


#endif /* NESTL_DETAIL_SELECT_TYPE_HPP */
