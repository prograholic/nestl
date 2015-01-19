/**
 * @file This file contains implementation of is_function template
 *
 * @note This file does not has any include guards
 */

template<typename>
struct is_function : public false_type
{
};

#if NESTL_HAS_VARIADIC_TEMPLATES

/**
 * @bug gcc-4.7.2 fails to compile specialization of is_function with ref qualifiers
 * so we explicitly forbide such specializations
 */
#if (NESTL_COMPILER != NESTL_COMPILER_GCC) || (NESTL_GCC_VERSION >= NESTL_COMPILER_GCC_48)

#	define NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS	1

#else /* (NESTL_COMPILER != NESTL_COMPILER_GCC) || (NESTL_GCC_VERSION >= NESTL_COMPILER_GCC_48) */

#	define NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS	0

#endif /* (NESTL_COMPILER != NESTL_COMPILER_GCC) || (NESTL_GCC_VERSION >= NESTL_COMPILER_GCC_48) */


template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...)> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...)> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const>  : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) volatile> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) volatile &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) volatile> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) volatile &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const volatile> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const volatile &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const volatile> : public true_type
{
};

#if NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const volatile &&> : public true_type
{
};

#endif /* NESTL_IS_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

#else /* NESTL_HAS_VARIADIC_TEMPLATES */


template<typename Res>
struct is_function<Res()> : public true_type
{
};

template<typename Res>
struct is_function<Res(...)> : public true_type
{
};


template<typename Res, typename Arg>
struct is_function<Res(Arg)> : public true_type
{
};

template<typename Res, typename Arg>
struct is_function<Res(Arg, ...)> : public true_type
{
};


template<typename Res, typename Arg0, typename Arg1>
struct is_function<Res(Arg0, Arg1)> : public true_type
{
};

template<typename Res, typename Arg0, typename Arg1>
struct is_function<Res(Arg0, Arg1, ...)> : public true_type
{
};


template<typename Res, typename Arg0, typename Arg1, typename Arg2>
struct is_function<Res(Arg0, Arg1, Arg2)> : public true_type
{
};

template<typename Res, typename Arg0, typename Arg1, typename Arg2>
struct is_function<Res(Arg0, Arg1, Arg2, ...)> : public true_type
{
};


template<typename Res, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
struct is_function<Res(Arg0, Arg1, Arg2, Arg3)> : public true_type
{
};

template<typename Res, typename Arg0, typename Arg1, typename Arg2, typename Arg3>
struct is_function<Res(Arg0, Arg1, Arg2, Arg3, ...)> : public true_type
{
};






#endif /* NESTL_HAS_VARIADIC_TEMPLATES */
