#ifndef NESTL_DETAIL_CLANG_HPP
#define NESTL_DETAIL_CLANG_HPP

#define NESTL_COMPILER                           NESTL_COMPILER_CLANG

#define NESTL_UNUSED                             __attribute__((unused))

#define NESTL_HAS_EXCEPTIONS                     (__EXCEPTIONS == 1)

#if __cplusplus < 201103L

#   error Please enable C++-11 mode

#endif /* __cplusplus < 201103L */

#define NESTL_HAS_NOEXCEPT                    1
#define NESTL_HAS_CONSTEXPR                   1

#define NESTL_CONSTEXPR                       constexpr
#define NESTL_NOEXCEPT_OPERATOR(x)            noexcept(x)
#define NESTL_NOEXCEPT_SPEC                   noexcept

#endif /* NESTL_DETAIL_GCC_HPP */
