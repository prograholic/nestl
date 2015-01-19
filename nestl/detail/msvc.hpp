#ifndef NESTL_DETAIL_MSVC_HPP
#define NESTL_DETAIL_MSVC_HPP


#define NESTL_COMPILER                           NESTL_COMPILER_MSVC


#define NESTL_COMPILER_MSVC_2008                 1500
#define NESTL_COMPILER_MSVC_2010                 1600
#define NESTL_COMPILER_MSVC_2012                 1700
#define NESTL_COMPILER_MSVC_2013                 1800

/**
 * @note MSVC by default supports c++-11.
 * But different versions have different features, so we disable C++11 by default
 */
#if !defined(NESTL_ENABLE_CXX11)
#   define NESTL_ENABLE_CXX11                    0
#endif /* NESTL_ENABLE_CXX11 */


#define NESTL_UNUSED

/**
 * @note We unconditionally declare NESTL_DECLTYPE as C++-11 keyword decltype
 * because we cannot provide emulation of decltype.
 *
 * @todo Boost has implementation of typeof for msvc compiler, we can take their implementation
 */
#define NESTL_DECLTYPE(x)                        delctype(x)

#if (_MSC_VER <= NESTL_COMPILER_MSVC_2008)

#   define NESTL_HAS_NOEXCEPT                    0
#   define NESTL_HAS_DECLTYPE                    0
#   define NESTL_HAS_STATIC_ASSERT               0
#   define NESTL_HAS_CONSTEXPR                   0
#   define NESTL_HAS_VARIADIC_TEMPLATES          0
#   define NESTL_HAS_RVALUE_REF                  0
#   define NESTL_HAS_EXPLICIT_OPERATOR           0
#   define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT     0
#   define NESTL_HAS_NATIVE_CHAR16_TYPE          1
#   define NESTL_HAS_NATIVE_CHAR32_TYPE          1
#   define NESTL_CONFIG_HAS_STATIC_ASSERT        1

#   define NESTL_CONSTEXPR
#   define NESTL_COMPILER_GENERATED_IMPL         {}
#   define NESTL_DELETED_IMPL
#   define NESTL_NOEXCEPT_OPERATOR(x)            true
#   define NESTL_NOEXCEPT_SPEC

#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2010)

#   define NESTL_HAS_NOEXCEPT                    1
#   define NESTL_HAS_DECLTYPE                    1
#   define NESTL_HAS_STATIC_ASSERT               1
#   define NESTL_HAS_CONSTEXPR                   0
#   define NESTL_HAS_VARIADIC_TEMPLATES          0
#   define NESTL_HAS_RVALUE_REF                  1
#   define NESTL_HAS_EXPLICIT_OPERATOR           0
#   define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT     0
#   define NESTL_HAS_NATIVE_CHAR16_TYPE          1
#   define NESTL_HAS_NATIVE_CHAR32_TYPE          1
#   define NESTL_CONFIG_HAS_STATIC_ASSERT        1

#   define NESTL_CONSTEXPR
#   define NESTL_COMPILER_GENERATED_IMPL         {}
#   define NESTL_DELETED_IMPL
#   define NESTL_NOEXCEPT_OPERATOR(x)            true
#   define NESTL_NOEXCEPT_SPEC

#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2012)
#   define NESTL_HAS_NOEXCEPT                    1
#   define NESTL_HAS_DECLTYPE                    1
#   define NESTL_HAS_STATIC_ASSERT               1
#   define NESTL_HAS_CONSTEXPR                   0
#   define NESTL_HAS_VARIADIC_TEMPLATES          0
#   define NESTL_HAS_RVALUE_REF                  1
#   define NESTL_HAS_EXPLICIT_OPERATOR           0
#   define NESTL_HAS_CXX11_ALIGNMENT_SUPPORT     0
#   define NESTL_HAS_NATIVE_CHAR16_TYPE          1
#   define NESTL_HAS_NATIVE_CHAR32_TYPE          1
#   define NESTL_CONFIG_HAS_STATIC_ASSERT        1

#   define NESTL_CONSTEXPR
#   define NESTL_COMPILER_GENERATED_IMPL         {}
#   define NESTL_DELETED_IMPL
#   define NESTL_NOEXCEPT_OPERATOR(x)            true
#   define NESTL_NOEXCEPT_SPEC


#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2013)

#   error msvc 2013 support is not implemented yet

#else /* MSC_VER */

#   error Unsupported compiler version, please update

#endif /* _MSC_VER */


#if NESTL_HAS_CXX11_ALIGNMENT_SUPPORT

#   define NESTL_ALIGNAS(expr, alignment)        alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)                   alignof(expr)

#else /* NESTL_HAS_CXX11_ALIGNMENT_SUPPORT */

#   define NESTL_ALIGNAS(expr, alignment)        expr
#   define NESTL_ALIGNOF(expr)                   __alignof(expr)

#endif /* NESTL_HAS_CXX11_ALIGNMENT_SUPPORT */


#if NESTL_ENABLE_CXX11
#   define NESTL_THROW_SPEC(x)
#   define NESTL_NOEXCEPT_OR_THROW_SPEC          NESTL_NOEXCEPT_SPEC
#else /* NESTL_ENABLE_CXX11 */

/**
 * @note MSVC ignores any exception specification except throw()
 */
#   define NESTL_THROW_SPEC(x)
#   define NESTL_NOEXCEPT_OR_THROW_SPEC          throw()
#endif /* NESTL_ENABLE_CXX11 */


#endif /* NESTL_DETAIL_MSVC_HPP */
