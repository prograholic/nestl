#ifndef NESTL_DETAIL_GCC_HPP
#define NESTL_DETAIL_GCC_HPP


#define NESTL_COMPILER                       NESTL_COMPILER_GCC

/**
 * @note we do not use __GNUC_PATCHLEVEL__ so both 4.7.4 and 4.7.0 are recognised as one version
 */
#define NESTL_GCC_VERSION                        (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)

#define NESTL_COMPILER_GCC_46                    40600
#define NESTL_COMPILER_GCC_47                    40700
#define NESTL_COMPILER_GCC_48                    40800
#define NESTL_COMPILER_GCC_49                    40900


#define NESTL_UNUSED                             __attribute__((unused))


#if defined(__GXX_EXPERIMENTAL_CXX0X__)

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
#   define NESTL_HAS_NATIVE_CHAR16_TYPE          1
#   define NESTL_HAS_NATIVE_CHAR32_TYPE          1
#   define NESTL_CONFIG_HAS_STATIC_ASSERT        1

#   if NESTL_GCC_VERSION > NESTL_COMPILER_GCC_47
#       define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT 1
#   else /* NESTL_GCC_VERSION > NESTL_COMPILER_GCC_47 */
#       define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT 0
#   endif /* NESTL_GCC_VERSION > NESTL_COMPILER_GCC_47 */

#   define NESTL_CONSTEXPR                       constexpr
#   define NESTL_COMPILER_GENERATED_IMPL         = default
#   define NESTL_DELETED_IMPL                    = delete
#   define NESTL_NOEXCEPT_OPERATOR(x)            noexcept(x)
#   define NESTL_NOEXCEPT_SPEC                   noexcept
#   define NESTL_DECLTYPE(x)                     decltype(x)
#   define NESTL_THROW_SPEC(x)
#   define NESTL_NOEXCEPT_OR_THROW_SPEC          NESTL_NOEXCEPT_SPEC

#else /* __GXX_EXPERIMENTAL_CXX0X__ */


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
#   define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT     0
#   define NESTL_HAS_NATIVE_CHAR16_TYPE          0
#   define NESTL_HAS_NATIVE_CHAR32_TYPE          0
#   define NESTL_CONFIG_HAS_STATIC_ASSERT        0

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

#   define NESTL_THROW_SPEC(x)                   throw(x)
#   define NESTL_NOEXCEPT_OR_THROW_SPEC          throw()

#endif /* __GXX_EXPERIMENTAL_CXX0X__ */


#if NESTL_HAS_CXX11_ALIGNMENT_SUPPORT

#   define NESTL_ALIGNAS(expr, alignment)        alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)                   alignof(expr)

#else /* NESTL_HAS_CXX11_ALIGNMENT_SUPPORT */

#   define NESTL_ALIGNAS(expr, alignment)        expr __attribute__((aligned (alignment)))
#   define NESTL_ALIGNOF(expr)                   __alignof__(expr)

#endif /* NESTL_HAS_CXX11_ALIGNMENT_SUPPORT */

#endif /* NESTL_DETAIL_GCC_HPP */
