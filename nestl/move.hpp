/**
 * @file Provides declaration of nestl::move function
 *
 * @note Implementation is taken from adobe ASL library
 */


#ifndef NESTL_MOVE_HPP
#define NESTL_MOVE_HPP

#include <nestl/config.hpp>

#include <nestl/detail/select_type.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/mpl.hpp>

namespace nestl
{

namespace detail
{



template <typename T>
struct class_has_move_assign
{
    NESTL_METHOD_CHECKER_EX(T, operator=, assign_operator);

    typedef has_assign_operator_member type;

    enum
    {
        value = type::value
    };
};


template<typename T>
struct has_move_assign : nestl::mpl::and_<nestl::is_class<T>, class_has_move_assign<T> >
{
};


class test_can_convert_anything
{
};

} // namespace detail

template <typename T>
struct move_from
{
    explicit move_from(T& x)
    : source(x)
    {
    }

    T& source;
};


template <typename T>
struct is_movable : nestl::mpl::and_<
                        nestl::is_convertible<nestl::move_from<T>, T>,
                        nestl::detail::has_move_assign<T>,
                        nestl::mpl::not_<nestl::is_convertible<nestl::detail::test_can_convert_anything, T> >
                    > { };


template <typename T,
          typename U = T,
          typename R = void*>
struct copy_sink : nestl::enable_if<
                        nestl::mpl::and_<
                            nestl::is_convertible<T, U>,
                            nestl::mpl::not_<is_movable<T> >
                        >,
                        R
                    >
{
};



template <typename T,
          typename U = T,
          typename R = void*>
struct move_sink : nestl::enable_if<
                        nestl::mpl::and_<
                            nestl::is_convertible<T, U>,
                            nestl::is_movable<T>
                        >,
                        R
                    >
{
};


template <typename T>
T move(T& x, typename move_sink<T>::type = 0)
{
    return T(move_from<T>(x));
}


template <typename T>
T& move(T& x, typename copy_sink<T>::type = 0)
{
    return x;
}

} // namespace nestl


#endif /* NESTL_MOVE_HPP */
