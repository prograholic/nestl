#ifndef NESTL_NUMERIC_CASTING_HPP
#define NESTL_NUMERIC_CASTING_HPP


#include <nestl/config.hpp>

#include <nestl/static_assert.hpp>
#include <nestl/numeric_limits.hpp>
#include <nestl/type_traits.hpp>

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

template <typename SrcInt, typename DstInt>
numeric_cast_status cast_unsigned_to_unsigned(SrcInt val) NESTL_NOEXCEPT_SPEC
{
    NESTL_STATIC_ASSERT(nestl::is_unsigned<SrcInt>::value, "SrcInt should be unsigned type");
    NESTL_STATIC_ASSERT(nestl::is_unsigned<DstInt>::value, "DstInt should be unsigned type");

    if (val > nestl::numeric_limits<DstInt>::max_value())
    {
        return cast_rejected_positive_overflow;
    }

    return cast_successful;
}


template <typename SrcInt, typename DstInt>
numeric_cast_status cast_unsigned_to_signed(SrcInt val) NESTL_NOEXCEPT_SPEC
{
    NESTL_STATIC_ASSERT(nestl::is_unsigned<SrcInt>::value, "SrcInt should be unsigned type");
    NESTL_STATIC_ASSERT(nestl::is_signed<DstInt>::value, "DstInt should be unsigned type");

    if (val > nestl::numeric_limits<DstInt>::max_value())
    {
        return cast_rejected_positive_overflow;
    }

    return cast_successful;
}

template <typename SrcInt, typename DstInt>
numeric_cast_status cast_signed_to_unsigned(SrcInt val) NESTL_NOEXCEPT_SPEC
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

template <typename SrcInt, typename DstInt>
numeric_cast_status cast_signed_to_signed(SrcInt val) NESTL_NOEXCEPT_SPEC
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

template <bool SrcIsUnsigned, bool DstIsUnsigned>
struct converter_helper;

template <>
struct converter_helper<true, true>
{
    template <typename SrcInt, typename DstInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        return cast_unsigned_to_unsigned<SrcInt, DstInt>(val);
    }
};

template <>
struct converter_helper<true, false>
{
    template <typename SrcInt, typename DstInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        return cast_unsigned_to_signed<SrcInt, DstInt>(val);
    }
};

template <>
struct converter_helper<false, true>
{
    template <typename SrcInt, typename DstInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        return cast_signed_to_unsigned<SrcInt, DstInt>(val);
    }
};

template <>
struct converter_helper<false, false>
{
    template <typename SrcInt, typename DstInt>
    static numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
    {
        return cast_signed_to_signed<SrcInt, DstInt>(val);
    }
};

} // namespace detail






template <typename SrcInt, typename DstInt>
numeric_cast_status cast_status(SrcInt val) NESTL_NOEXCEPT_SPEC
{
    typedef detail::converter_helper<nestl::is_unsigned<SrcInt>::value, nestl::is_unsigned<DstInt>::value> converter_helper;

    return converter_helper::cast_status<SrcInt, DstInt>(val);
}



} // namespace numeric


} // namespace nestl


#endif /* NESTL_NUMERIC_CASTING_HPP */
