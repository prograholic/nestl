#ifndef NESTL_DETAIL_CLANG_HPP
#define NESTL_DETAIL_CLANG_HPP


#define NESTL_COMPILER                                  NESTL_COMPILER_CLANG


#define NESTL_UNUSED                                    __attribute__((unused))


#if __cplusplus >= 201103L

#   if !defined (NESTL_CONFIG_HAS_CXX11)
#       define NESTL_CONFIG_HAS_CXX11                   1
#   endif /* NESTL_CONFIG_HAS_CXX11 */

#   define NESTL_CONFIG_HAS_NOEXCEPT                    1
#   define NESTL_CONFIG_HAS_DECLTYPE                    1
#   define NESTL_CONFIG_HAS_STATIC_ASSERT               1
#   define NESTL_CONFIG_HAS_CONSTEXPR                   1
#   define NESTL_CONFIG_HAS_VARIADIC_TEMPLATES          1
#   define NESTL_CONFIG_HAS_RVALUE_REF                  1
#   define NESTL_CONFIG_HAS_EXPLICIT_OPERATOR           1
#   define NESTL_CONFIG_HAS_INITIALIZER_LIST_HEADER     1
#   define NESTL_CONFIG_HAS_CXX11_ALIGNMENT_SUPPORT     1

#   define NESTL_CONSTEXPR                              constexpr
#   define NESTL_COMPILER_GENERATED_IMPL                = default
#   define NESTL_DELETED_IMPL                           = delete
#   define NESTL_NOEXCEPT_OPERATOR(x)                   noexcept(x)
#   define NESTL_NOEXCEPT_SPEC                          noexcept
#   define NESTL_DECLTYPE(x)                            decltype(x)
#   define NESTL_THROW_SPEC(x)
#   define NESTL_NOEXCEPT_OR_THROW_SPEC                 NESTL_NOEXCEPT_SPEC



#else /* __cplusplus >= 201103L */

#   if defined(NESTL_CONFIG_HAS_CXX11)
#       error Conflicting configuration: "NESTL_CONFIG_HAS_CXX11 is defined"
#   endif

#   define NESTL_CONSTEXPR
#   define NESTL_COMPILER_GENERATED_IMPL                {}
#   define NESTL_DELETED_IMPL
/**
 * @note Since we do not have noexcept support,
 * we assume that every operation may throw exception.
 * But actually we should provide our custom noexcept operator
 */
#   define NESTL_NOEXCEPT_OPERATOR(x)                   true
#   define NESTL_NOEXCEPT_SPEC

/**
 * @note GCC's typeof is different from c++11 decltype
 */
#   define NESTL_DECLTYPE(x)                            typeof(x)

#   define NESTL_THROW_SPEC(x)                          throw(x)
#   define NESTL_NOEXCEPT_OR_THROW_SPEC                 throw()



#endif /* __cplusplus >= 201103L */


#if defined(NESTL_CONFIG_HAS_CXX11_ALIGNMENT_SUPPORT)

#   define NESTL_ALIGNAS(expr, alignment)               alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)                          alignof(expr)

#else /* defined(NESTL_CONFIG_HAS_CXX11_ALIGNMENT_SUPPORT) */

#   define NESTL_ALIGNAS(expr, alignment)               expr __attribute__((aligned (alignment)))
#   define NESTL_ALIGNOF(expr)                          __alignof__(expr)

#endif /* defined(NESTL_CONFIG_HAS_CXX11_ALIGNMENT_SUPPORT) */

#endif /* NESTL_DETAIL_GCC_HPP */
