#ifndef NESTL_DETAIL_MSVC_HPP
#define NESTL_DETAIL_MSVC_HPP


#define NESTL_COMPILER                           NESTL_COMPILER_MSVC


#define NESTL_COMPILER_MSVC_2008                 1500
#define NESTL_COMPILER_MSVC_2010                 1600
#define NESTL_COMPILER_MSVC_2012                 1700
#define NESTL_COMPILER_MSVC_2013                 1800
#define NESTL_COMPILER_MSVC_2015                 1900

// NOTE: VS2017 has two compiler versions 1910 and 1911 depending on installed updates
#define NESTL_COMPILER_MSVC_2017                 1911

#define NESTL_UNUSED

#if !defined(_CPPUNWIND)
#   define NESTL_HAS_EXCEPTIONS                  0
#else /* _CPPUNWIND */
#   define NESTL_HAS_EXCEPTIONS                  1
#endif /* _CPPUNWIND */



#if (_MSC_VER < NESTL_COMPILER_MSVC_2013)

#   error Your compiler too old, please use C++-11 compliant compiler

#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2013)

#   define NESTL_HAS_NOEXCEPT                     0
#   define NESTL_HAS_CONSTEXPR                    0 

#   define NESTL_CONSTEXPR
#   define NESTL_NOEXCEPT_OPERATOR(x)             true
#   define NESTL_NOEXCEPT_SPEC                    throw()

#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2015)
#   define NESTL_HAS_NOEXCEPT                     1
#   define NESTL_HAS_CONSTEXPR                    1

#   define NESTL_CONSTEXPR                        constexpr
#   define NESTL_NOEXCEPT_OPERATOR(x)             noexcept(x)
#   define NESTL_NOEXCEPT_SPEC                    noexcept

#elif (_MSC_VER <= NESTL_COMPILER_MSVC_2017)
#   define NESTL_HAS_NOEXCEPT                     1
#   define NESTL_HAS_CONSTEXPR                    1

#   define NESTL_CONSTEXPR                        constexpr
#   define NESTL_NOEXCEPT_OPERATOR(x)             noexcept(x)
#   define NESTL_NOEXCEPT_SPEC                    noexcept

#else /* MSC_VER */

#   error Unsupported compiler version, please update

#endif /* _MSC_VER */



#endif /* NESTL_DETAIL_MSVC_HPP */
