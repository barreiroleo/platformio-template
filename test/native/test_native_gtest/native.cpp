#include <gtest/gtest.h>

TEST(NativeTest, BasicLogic)
{
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
    EXPECT_EQ(1, 1);
    EXPECT_NE(1, 0);
}

auto main(int argc, char **argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
