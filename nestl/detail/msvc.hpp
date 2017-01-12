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

// put MSVC 2015-specific macros and definitions there

#else /* MSC_VER */

#   error Unsupported compiler version, please update

#endif /* _MSC_VER */

#if defined _CPPUNWIND
#   define NESTL_HAS_EXCEPTIONS                  1
#else /* _CPPUNWIND */
#   define NESTL_HAS_EXCEPTIONS                  0
#endif /* _CPPUNWIND */

#endif /* NESTL_DETAIL_MSVC_HPP */
