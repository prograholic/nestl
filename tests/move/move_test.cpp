#include "tests/move/move_test.hpp"

#include <nestl/noncopyable.hpp>

namespace nestl
{
namespace test
{
#if NESTL_HAS_RVALUE_REF
struct moveable : private nestl::noncopyable
{
    explicit moveable(int value) NESTL_NOEXCEPT_SPEC:
        m_value(value)
    {
    }

    moveable(moveable&& other) NESTL_NOEXCEPT_SPEC
        : m_value(other.m_value)
    {
        other.m_value = 0;
    }

    int get_value() const NESTL_NOEXCEPT_SPEC
    {
        return m_value;
    }

    moveable& operator=(moveable&& other) NESTL_NOEXCEPT_SPEC
    {
        m_value = other.m_value;
        other.m_value = 0;

        return *this;
    }


private:
    int m_value;
};

#if 0

TEST_F(MoveTestBase, CheckMove)
{
    moveable src(5);

    moveable dst = nestl::move(src);

    EXPECT_EQ(5, dst.get_value());
    EXPECT_EQ(0, src.get_value());
}

#endif //0

#endif /* NESTL_HAS_RVALUE_REF */

} // namespace test

} // namespace nestl
