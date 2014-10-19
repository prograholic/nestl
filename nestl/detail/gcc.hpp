#ifndef NESTL_DETAIL_GCC_HPP
#define NESTL_DETAIL_GCC_HPP


#define NESTL_COMPILER NESTL_COMPILER_GCC


#define NESTL_UNUSED                         __attribute__((unused))


#if defined(__GXX_EXPERIMENTAL_CXX0X__)

#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11        1
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 0
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 0"
#   endif

#   define NESTL_HAS_NOEXCEPT                1
#   define NESTL_HAS_DECLTYPE                1
#   define NESTL_HAS_STATIC_ASSERT           1
#   define NESTL_HAS_CONSTEXPR               1
#   define NESTL_HAS_VARIADIC_TEMPLATES      1
#   define NESTL_HAS_RVALUE_REF              1
#   define NESTL_HAS_EXPLICIT_OPERATOR       1
#   define NESTL_HAS_INITIALIZER_LIST_HEADER 1

#   define NESTL_CONSTEXPR                   constexpr
#   define NESTL_COMPILER_GENERATED_IMPL     = default
#   define NESTL_DELETED_IMPL                = delete
#   define NESTL_NOEXCEPT_OPERATOR(x)        noexcept(x)
#   define NESTL_NOEXCEPT_SPEC               noexcept
#   define NESTL_DECLTYPE(x)                 decltype(x)
#   define NESTL_ALIGNAS(expr, alignment)    alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)               alignof(expr)


#else /* __GXX_EXPERIMENTAL_CXX0X__ */


#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11            0
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 1
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 1"
#   endif

#   define NESTL_HAS_NOEXCEPT                0
#   define NESTL_HAS_DECLTYPE                0
#   define NESTL_HAS_STATIC_ASSERT           0
#   define NESTL_HAS_CONSTEXPR               0
#   define NESTL_HAS_VARIADIC_TEMPLATES      0
#   define NESTL_HAS_RVALUE_REF              0
#   define NESTL_HAS_EXPLICIT_OPERATOR       0
#   define NESTL_HAS_INITIALIZER_LIST_HEADER 0

#   define NESTL_CONSTEXPR
#   define NESTL_COMPILER_GENERATED_IMPL     {}
#   define NESTL_DELETED_IMPL
/**
 * @note Since we do not have noexcept support,
 * we assume that every operation may throw exception.
 * But actually we should provide our custom noexcept operator
 */
#   define NESTL_NOEXCEPT_OPERATOR(x)        true
#   define NESTL_NOEXCEPT_SPEC

/**
 * @note GCC's typeof is different from c++11 decltype
 */
#   define NESTL_DECLTYPE(x)                 typeof(x)

#   define NESTL_ALIGNAS(expr, alignment)    expr __attribute__((aligned (alignment)))
#   define NESTL_ALIGNOF(expr)               __alignof__(expr)


#endif /* __GXX_EXPERIMENTAL_CXX0X__ */


#if defined(NESTL_ENABLE_CXX11)



#else /* NESTL_ENABLE_CXX11 */



#endif /* NESTL_ENABLE_CXX11 */


#endif /* NESTL_DETAIL_GCC_HPP */
