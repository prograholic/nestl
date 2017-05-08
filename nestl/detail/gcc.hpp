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

#define NESTL_HAS_EXCEPTIONS                     (__EXCEPTIONS == 1)


#if !defined(__GXX_EXPERIMENTAL_CXX0X__)

#error Please enable C++-11 for your compiler

#endif /* defined(__GXX_EXPERIMENTAL_CXX0X__) */ 

#define NESTL_HAS_NOEXCEPT                    1
#define NESTL_HAS_CONSTEXPR                   1

#define NESTL_CONSTEXPR                       constexpr
#define NESTL_NOEXCEPT_OPERATOR(x)            noexcept(x)
#define NESTL_NOEXCEPT_SPEC                   noexcept

#endif /* NESTL_DETAIL_GCC_HPP */
