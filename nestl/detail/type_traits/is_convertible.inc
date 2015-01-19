/**
 * @file This file contains implementation of is_convertible template
 *
 * @note This file does not have any include guards
 */


namespace detail
{

} // namespace detail


template<typename From, typename To>
struct is_convertible : public nestl::false_type
{
};
