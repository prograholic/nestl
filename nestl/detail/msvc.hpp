#ifndef NESTL_DETAIL_MSVC_HPP
#define NESTL_DETAIL_MSVC_HPP


#define NESTL_COMPILER                           NESTL_COMPILER_MSVC


#define NESTL_COMPILER_MSVC_2008                 1500
#define NESTL_COMPILER_MSVC_2010                 1600
#define NESTL_COMPILER_MSVC_2012                 1700
#define NESTL_COMPILER_MSVC_2013                 1800
#define NESTL_COMPILER_MSVC_2015                 1900


#define NESTL_UNUSED


#if (_MSC_VER <= NESTL_COMPILER_MSVC_2013)

#   error older versions of msvc are not supported

#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2015)

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

#else /* MSC_VER */

#   error Unsupported compiler version, please update

#endif /* _MSC_VER */

#if defined _CPPUNWIND
#   define NESTL_HAS_EXCEPTIONS                  1
#else /* _CPPUNWIND */
#   define NESTL_HAS_EXCEPTIONS                  0
#endif /* _CPPUNWIND */

#endif /* NESTL_DETAIL_MSVC_HPP */
