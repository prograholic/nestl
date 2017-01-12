#include "tests/move/move_test.hpp"

#include <nestl/noncopyable.hpp>

namespace nestl
{
namespace test
{
struct moveable : private nestl::noncopyable
{
    explicit moveable(int value) noexcept:
        m_value(value)
    {
    }

    moveable(moveable&& other) noexcept
        : m_value(other.m_value)
    {
        other.m_value = 0;
    }

    int get_value() const noexcept
    {
        return m_value;
    }

    moveable& operator=(moveable&& other) noexcept
    {
        m_value = other.m_value;
        other.m_value = 0;

        return *this;
    }


private:
    int m_value;
};

TEST_F(MoveTestBase, CheckMove)
{
    moveable src(5);

    moveable dst = nestl::move(src);

    EXPECT_EQ(5, dst.get_value());
    EXPECT_EQ(0, src.get_value());
}

} // namespace test

} // namespace nestl
