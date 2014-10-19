#ifndef NESTL_DETAIL_MSVC_HPP
#define NESTL_DETAIL_MSVC_HPP


#define NESTL_COMPILER                       NESTL_COMPILER_MSVC


#define NESTL_COMPILER_MSVC_2008             1500
#define NESTL_COMPILER_MSVC_2010             1600
#define NESTL_COMPILER_MSVC_2012             1700
#define NESTL_COMPILER_MSVC_2013             1800

/**
 * @note MSVC by default supports c++-11.
 * But different versions have different features, so explic
 */
#if !defined(NESTL_ENABLE_CXX11)
#   define NESTL_ENABLE_CXX11                0
#endif /* NESTL_ENABLE_CXX11 */


#define NESTL_UNUSED

/**
 * @note We unconditionally declare NESTL_DECLTYPE as C++-11 keyword decltype
 * because we cannot provide emulation of decltype.
 *
 * @todo Boost has implementation of typeof for msvc compiler, we can take their implementation
 */
#define NESTL_DECLTYPE(x)                    delctype(x)

#if (MSC_VER <= NESTL_COMPILER_MSVC_2008)

#   define NESTL_HAS_NOEXCEPT                1
#   define NESTL_HAS_DECLTYPE                1
#   define NESTL_HAS_STATIC_ASSERT           0
#   define NESTL_HAS_CONSTEXPR               0
#   define NESTL_HAS_VARIADIC_TEMPLATES      0
#   define NESTL_HAS_RVALUE_REF              0
#   define NESTL_HAS_EXPLICIT_OPERATOR       0
#   define NESTL_HAS_INITIALIZER_LIST_HEADER 0

#   define NESTL_CONSTEXPR                   
#   define NESTL_COMPILER_GENERATED_IMPL     {}
#   define NESTL_DELETED_IMPL                
#   define NESTL_NOEXCEPT_OPERATOR(x)        true
#   define NESTL_NOEXCEPT_SPEC               
#   define NESTL_ALIGNAS(expr, alignment)    expr
#   define NESTL_ALIGNOF(expr)               __alignof(expr)

#elif (MSC_VER <= NESTL_COMPILER_MSVC_2010)
#elif (MSC_VER <= NESTL_COMPILER_MSVC_2012)
#elif (MSC_VER <= NESTL_COMPILER_MSVC_2013)
#else /* MSC_VER */
#   error Unsupported compiler version, please update
#endif /* _MSC_VER */








#endif /* NESTL_DETAIL_MSVC_HPP */
