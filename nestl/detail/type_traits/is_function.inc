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



template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...)> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...)> : public true_type
{
};

#if NESTL_FUNCTION_CAN_HAVE_QUALIFIERS

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const>  : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) volatile> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) volatile &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) volatile> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) volatile &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const volatile> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes...) const volatile &&> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const volatile> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const volatile &> : public true_type
{
};

template<typename Res, typename... ArgTypes>
struct is_function<Res(ArgTypes..., ...) const volatile &&> : public true_type
{
};

#endif /* NESTL_FUNCTION_CAN_HAVE_REF_QUALIFIERS */

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
