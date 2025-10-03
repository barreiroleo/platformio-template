#include <unity.h>

void setUp()
{
}

void tearDown()
{
}

void test_basic_logic()
{
    TEST_ASSERT_TRUE(true);
    TEST_ASSERT_FALSE(false);
    TEST_ASSERT_EQUAL(1, 1);
    TEST_ASSERT_NOT_EQUAL(1, 0);
}

auto main() -> int
{
    UNITY_BEGIN();
    RUN_TEST(test_basic_logic);
    return UNITY_END();
}
