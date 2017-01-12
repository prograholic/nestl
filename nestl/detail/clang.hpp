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

#else /* __cplusplus >= 201103L */


#   if !defined (NESTL_ENABLE_CXX11)
#       define NESTL_ENABLE_CXX11                0
#   endif /* NESTL_ENABLE_CXX11 */

#   if NESTL_ENABLE_CXX11 == 1
#       error Conflicting configuration "NESTL_ENABLE_CXX11 == 1"
#   endif


#endif /* __cplusplus >= 201103L */


#   define NESTL_ALIGNAS(expr, alignment)        alignas(alignment) expr
#   define NESTL_ALIGNOF(expr)                   alignof(expr)

#endif /* NESTL_DETAIL_GCC_HPP */
