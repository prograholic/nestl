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

#if NESTL_HAS_RVALUE_REF

namespace nestl
{

template<typename T>
NESTL_CONSTEXPR typename nestl::remove_reference<T>::type&&
move(T&& t) NESTL_NOEXCEPT_SPEC
{
    return static_cast<typename nestl::remove_reference<T>::type&&>(t);
}


} // namespace nestl

#define NESTL_MOVE_IF_SUPPORTED(x) nestl::move(x)

#else /* NESTL_HAS_RVALUE_REF */

#define NESTL_MOVE_IF_SUPPORTED(x) x

#endif /* NESTL_HAS_RVALUE_REF */

#if 0

namespace nestl
{

namespace detail
{



template <typename T>
struct class_has_move_assign
{
    NESTL_HAS_METHOD_EX(T, operator=, assign_operator);

    //typedef nestl::class_traits<T> class_traits_type;

    //NESTL_HAS_METHOD(T, move_assign);

    typedef has_assign_operator_member type;
    //typedef has_move_assign_member type;

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


#define NESTL_RV_REF(Type) nestl::move_from<Type>


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


#if 0

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

#endif //0

template <typename T>
T& move(T& x)
{
    return x;
}

} // namespace nestl

#endif //0
#endif /* NESTL_MOVE_HPP */
