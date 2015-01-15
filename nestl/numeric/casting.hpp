#ifndef NESTL_NUMERIC_CASTING_HPP
#define NESTL_NUMERIC_CASTING_HPP


#include <nestl/config.hpp>

#include <nestl/static_assert.hpp>
#include <nestl/numeric_limits.hpp>
#include <nestl/type_traits.hpp>
#include <nestl/system_error.hpp>

namespace nestl
{


namespace numeric
{

enum numeric_cast_status
{
    /// Destination type can represent source type value and their both equal
    cast_successful,

    /// Destination type cannot represent source type value, because source type value lesser then
    cast_rejected_negative_overflow,
    cast_rejected_positive_overflow,
};


namespace detail
{

template <bool SrcIsUnsigned, bool DstIsUnsigned>
struct converter_helper;

template <>
struct converter_helper<true, true>
{
    template <typename DstInt, typename SrcInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        NESTL_STATIC_ASSERT(nestl::is_unsigned<SrcInt>::value, "SrcInt should be unsigned type");
        NESTL_STATIC_ASSERT(nestl::is_unsigned<DstInt>::value, "DstInt should be unsigned type");

        if (val > nestl::numeric_limits<DstInt>::max_value())
        {
            return cast_rejected_positive_overflow;
        }

        return cast_successful;
    }
};

template <>
struct converter_helper<false, true>
{
    template <typename DstInt, typename SrcInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        NESTL_STATIC_ASSERT(nestl::is_unsigned<SrcInt>::value, "SrcInt should be unsigned type");
        NESTL_STATIC_ASSERT(nestl::is_signed<DstInt>::value, "DstInt should be unsigned type");

        if (val > nestl::numeric_limits<DstInt>::max_value())
        {
            return cast_rejected_positive_overflow;
        }

        return cast_successful;
    }
};

template <>
struct converter_helper<true, false>
{
    template <typename DstInt, typename SrcInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        NESTL_STATIC_ASSERT(nestl::is_signed<SrcInt>::value, "SrcInt should be unsigned type");
        NESTL_STATIC_ASSERT(nestl::is_unsigned<DstInt>::value, "DstInt should be unsigned type");

        if (val < 0)
        {
            return cast_rejected_negative_overflow;
        }

        if (val > nestl::numeric_limits<DstInt>::max_value())
        {
            return cast_rejected_positive_overflow;
        }

        return cast_successful;
    }
};

template <>
struct converter_helper<false, false>
{
    template <typename DstInt, typename SrcInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        NESTL_STATIC_ASSERT(nestl::is_signed<SrcInt>::value, "SrcInt should be unsigned type");
        NESTL_STATIC_ASSERT(nestl::is_signed<DstInt>::value, "DstInt should be unsigned type");

        if (val < 0)
        {
            if (val < nestl::numeric_limits<DstInt>::min_value())
            {
                return cast_rejected_negative_overflow;
            }

            return cast_successful;
        }

        if (val > nestl::numeric_limits<DstInt>::max_value())
        {
            return cast_rejected_positive_overflow;
        }

        return cast_successful;
    }
};

} // namespace detail






template <typename DstInt, typename SrcInt>
numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
{
    typedef detail::converter_helper<nestl::is_unsigned<DstInt>::value, nestl::is_unsigned<SrcInt>::value> converter_helper;

    return converter_helper::template cast_status<DstInt>(val);
}


template <typename DstInt, typename SrcInt>
nestl::error_condition safe_cast(SrcInt val, DstInt& res) NESTL_NOEXCEPT_SPEC
{
    if (cast_status<DstInt>(val) != cast_successful)
    {
        return nestl::error_condition(nestl::errc::invalid_argument);
    }

    res = static_cast<DstInt>(val);

    return nestl::error_condition();
}



template <typename DstInt, typename SrcInt>
DstInt unsafe_cast(SrcInt val) NESTL_NOEXCEPT_SPEC
{
    NESTL_ASSERT(cast_status<DstInt>(val) == cast_successful);

    return static_cast<DstInt>(val);
}


} // namespace numeric


} // namespace nestl


#endif /* NESTL_NUMERIC_CASTING_HPP */
