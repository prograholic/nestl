#ifndef NESTL_DETAIL_CLANG_HPP
#define NESTL_DETAIL_CLANG_HPP


#define NESTL_COMPILER                           NESTL_COMPILER_CLANG


#define NESTL_UNUSED                             __attribute__((unused))


#if __cplusplus >= 201103L

#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11                1
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 0
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 0"
#   endif

#   define NESTL_HAS_NOEXCEPT                    1
#   define NESTL_HAS_DECLTYPE                    1
#   define NESTL_HAS_STATIC_ASSERT               1
#   define NESTL_HAS_CONSTEXPR                   1
#   define NESTL_HAS_VARIADIC_TEMPLATES          1
#   define NESTL_HAS_RVALUE_REF                  1
#   define NESTL_HAS_EXPLICIT_OPERATOR           1
#   define NESTL_HAS_INITIALIZER_LIST_HEADER     1
#   define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT     1

#   define NESTL_CONSTEXPR                       constexpr
#   define NESTL_COMPILER_GENERATED_IMPL         = default
#   define NESTL_DELETED_IMPL                    = delete
#   define NESTL_NOEXCEPT_OPERATOR(x)            noexcept(x)
#   define NESTL_NOEXCEPT_SPEC                   noexcept
#   define NESTL_DECLTYPE(x)                     decltype(x)


#else /* __cplusplus >= 201103L */


#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11                0
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 1
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 1"
#   endif

#   define NESTL_HAS_NOEXCEPT                    0
#   define NESTL_HAS_DECLTYPE                    0
#   define NESTL_HAS_STATIC_ASSERT               0
#   define NESTL_HAS_CONSTEXPR                   0
#   define NESTL_HAS_VARIADIC_TEMPLATES          0
#   define NESTL_HAS_RVALUE_REF                  0
#   define NESTL_HAS_EXPLICIT_OPERATOR           0
#   define NESTL_HAS_INITIALIZER_LIST_HEADER     0
#   define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT     0

#   define NESTL_CONSTEXPR
#   define NESTL_COMPILER_GENERATED_IMPL         {}
#   define NESTL_DELETED_IMPL
/**
 * @note Since we do not have noexcept support,
 * we assume that every operation may throw exception.
 * But actually we should provide our custom noexcept operator
 */
#   define NESTL_NOEXCEPT_OPERATOR(x)            true
#   define NESTL_NOEXCEPT_SPEC

/**
 * @note GCC's typeof is different from c++11 decltype
 */
#   define NESTL_DECLTYPE(x)                     typeof(x)



#endif /* __cplusplus >= 201103L */


#if NESTL_HAS_CXX11_ALIGNMENT_SUPPORT

#   define NESTL_ALIGNAS(expr, alignment)        alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)                   alignof(expr)

#else /* NESTL_HAS_CXX11_ALIGNMENT_SUPPORT */

#   define NESTL_ALIGNAS(expr, alignment)        expr __attribute__((aligned (alignment)))
#   define NESTL_ALIGNOF(expr)                   __alignof__(expr)

#endif /* NESTL_HAS_CXX11_ALIGNMENT_SUPPORT */

#endif /* NESTL_DETAIL_GCC_HPP */
