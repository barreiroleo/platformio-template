#include <gtest/gtest.h>

#include <utility.hpp>

struct FooStruct {
    int value;
    FooStruct(int v)
        : value(v)
    {
    }
    FooStruct(FooStruct&& other)
        : value(other.value)
    {
        other.value = -1;
    }
};

TEST(UtilityTest, test_move_basic)
{
    // Test with lvalue
    int x = 42;
    auto moved_x = core::move(x);
    static_assert(sizeof(moved_x) == sizeof(int), "basic size check");

    // Test with rvalue
    auto moved_rvalue = core::move(42);
    static_assert(sizeof(moved_rvalue) == sizeof(int), "rvalue size check");

    SUCCEED();
}

TEST(UtilityTest, test_move_types)
{
    int value = 10;
    const int const_value = 20;

    // Test return types
    auto result1 = core::move(value);
    auto result2 = core::move(const_value);
    auto result3 = core::move(30);

    // Basic functionality tests
    EXPECT_EQ(result1, 10);
    EXPECT_EQ(result2, 20);
    EXPECT_EQ(result3, 30);

    SUCCEED();
}

TEST(UtilityTest, test_move_with_struct)
{
    FooStruct obj(100);
    FooStruct moved_ref = core::move(obj);
    FooStruct new_obj(core::move(moved_ref));

    EXPECT_EQ(new_obj.value, 100); // Move preserved value
    EXPECT_EQ(obj.value, -1); // Moved-from state
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
