/**
 * @file This file contains implementation of is_integral template
 *
 * @note This file does not has any include guards
 */

namespace detail
{

template<typename>
struct is_integral_helper : public false_type
{
};

template<>
struct is_integral_helper<bool> : public true_type
{
};

template<>
struct is_integral_helper<char> : public true_type
{
};

template<>
struct is_integral_helper<signed char> : public true_type
{
};

template<>
struct is_integral_helper<unsigned char> : public true_type
{
};

template<>
struct is_integral_helper<wchar_t> : public true_type
{
};

#if NESTL_HAS_NATIVE_CHAR16_TYPE

template<>
struct is_integral_helper<char16_t> : public true_type
{
};

#endif /* NESTL_HAS_NATIVE_CHAR16_TYPE */

#if NESTL_HAS_NATIVE_CHAR32_TYPE

template<>
struct is_integral_helper<char32_t> : public true_type
{
};

#endif /* NESTL_HAS_NATIVE_CHAR32_TYPE */

template<>
struct is_integral_helper<short> : public true_type
{
};

template<>
struct is_integral_helper<unsigned short> : public true_type
{
};

template<>
struct is_integral_helper<int> : public true_type
{
};

template<>
struct is_integral_helper<unsigned int> : public true_type
{
};

template<>
struct is_integral_helper<long> : public true_type
{
};

template<>
struct is_integral_helper<unsigned long> : public true_type
{
};

template<>
struct is_integral_helper<long long> : public true_type
{
};

template<>
struct is_integral_helper<unsigned long long> : public true_type
{
};

} // namespace detail

template<typename T>
struct is_integral : public detail::is_integral_helper<typename remove_cv<T>::type>::type
{
};
