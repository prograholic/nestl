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

#if defined __cpp_exceptions
#   define NESTL_HAS_EXCEPTIONS                  1
#else /* __cpp_exceptions */
#   define NESTL_HAS_EXCEPTIONS                  0
#endif /* __cpp_exceptions */

#if defined(__GXX_EXPERIMENTAL_CXX0X__)

#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11                1
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 0
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 0"
#   endif

#else /* __GXX_EXPERIMENTAL_CXX0X__ */


#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11                0
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 1
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 1"
#   endif

#endif /* __GXX_EXPERIMENTAL_CXX0X__ */

#   define NESTL_ALIGNAS(expr, alignment)        alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)                   alignof(expr)

#endif /* NESTL_DETAIL_GCC_HPP */
