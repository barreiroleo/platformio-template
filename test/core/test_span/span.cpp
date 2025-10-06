#include <gtest/gtest.h>

#include <span.hpp>

#include <numeric>

class SpanTest : public ::testing::Test {
protected:
    void SetUp() override { }
    void TearDown() override { }

    int m_array[5] { 1, 2, 3, 4, 5 };

    static constexpr int m_const_array[3] = { 10, 20, 30 };
};

// Constructors and Assignment Operators
TEST_F(SpanTest, test_default_constructor)
{
    {
        // Runtime tests
        core::span<int> span {};
        EXPECT_EQ(span.size(), 0);
        EXPECT_TRUE(span.empty());
        EXPECT_EQ(span.data(), nullptr);
    }
    {
        // Compile-time tests
        constexpr core::span<int> span {};
        static_assert(span.size() == 0);
        static_assert(span.empty());
        static_assert(span.data() == nullptr);
    }
}

TEST_F(SpanTest, test_copy_constructor)
{
    {
        // Runtime tests
        core::span<int> original_span(m_array, core::size(m_array));
        core::span<int> copied_span(original_span);

        EXPECT_EQ(copied_span.size(), original_span.size());
        EXPECT_EQ(copied_span.data(), original_span.data());
        EXPECT_FALSE(copied_span.empty());

        for (size_t i = 0; i < copied_span.size(); ++i) {
            EXPECT_EQ(copied_span[i], original_span[i]);
            EXPECT_EQ(copied_span[i], m_array[i]);
        }
    }
    {
        // Compile-time tests
        constexpr core::span<const int> original_span(m_const_array, core::size(m_const_array));
        constexpr core::span<const int> copied_span(original_span);

        static_assert(copied_span.size() == original_span.size());
        static_assert(copied_span.data() == original_span.data());
        static_assert(!copied_span.empty());
    }
}

TEST_F(SpanTest, test_copy_assignment)
{
    {
        // Runtime tests
        core::span<int> original_span(m_array, core::size(m_array));
        core::span<int> assigned_span {};

        assigned_span = original_span;

        EXPECT_EQ(assigned_span.size(), original_span.size());
        EXPECT_EQ(assigned_span.data(), original_span.data());
        EXPECT_FALSE(assigned_span.empty());

        for (size_t i = 0; i < assigned_span.size(); ++i) {
            EXPECT_EQ(assigned_span[i], original_span[i]);
            EXPECT_EQ(assigned_span[i], m_array[i]);
        }
    }
    {
        // Compile-time tests
        constexpr core::span<const int> original_span(m_const_array, 3);
        constexpr auto assigned_span = original_span;

        static_assert(assigned_span.size() == original_span.size());
        static_assert(assigned_span.data() == original_span.data());
        static_assert(!assigned_span.empty());
    }
}

TEST_F(SpanTest, test_pointer_size_constructor)
{
    {
        // Non-empty span
        core::span<int> span(m_array, core::size(m_array));

        EXPECT_EQ(span.size(), core::size(m_array));
        EXPECT_EQ(span.data(), m_array);
        EXPECT_FALSE(span.empty());

        for (size_t i = 0; i < span.size(); ++i) {
            EXPECT_EQ(span[i], m_array[i]);
        }
    }
    {
        // Empty span
        core::span<int> span(nullptr, 0);

        EXPECT_EQ(span.size(), 0);
        EXPECT_EQ(span.data(), nullptr);
        EXPECT_TRUE(span.empty());
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array, core::size(m_const_array));

        static_assert(span.size() == core::size(m_const_array));
        static_assert(span.data() == m_const_array);
        static_assert(!span.empty());
    }
}

TEST_F(SpanTest, test_array_constructor)
{
    {
        // Mutable array
        core::span<int> span(m_array);

        EXPECT_EQ(span.size(), core::size(m_array));
        EXPECT_EQ(span.data(), m_array);
        EXPECT_FALSE(span.empty());

        for (size_t i = 0; i < span.size(); ++i) {
            EXPECT_EQ(span[i], m_array[i]);
        }
    }
    {
        // Const array
        core::span<const int> span(m_const_array);

        EXPECT_EQ(span.size(), core::size(m_const_array));
        EXPECT_EQ(span.data(), m_const_array);
        EXPECT_FALSE(span.empty());

        for (size_t i = 0; i < span.size(); ++i) {
            EXPECT_EQ(span[i], m_const_array[i]);
        }
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);

        static_assert(span.size() == core::size(m_const_array));
        static_assert(span.data() == m_const_array);
        static_assert(!span.empty());
    }
}

// Iterator Tests
TEST_F(SpanTest, test_iterators_begin)
{
    {
        // Non-empty span
        core::span<int> span(m_array);

        auto it = span.begin();
        EXPECT_EQ(it, m_array);
        EXPECT_EQ(*it, m_array[0]);

        auto cit = span.cbegin();
        EXPECT_EQ(cit, m_array);
        EXPECT_EQ(*cit, m_array[0]);
        static_assert(std::is_same_v<decltype(cit), core::span<int>::const_iterator>);
    }
    {
        // Empty span
        core::span<int> empty_span {};

        auto it = empty_span.begin();
        EXPECT_EQ(it, nullptr);
        EXPECT_EQ(it, empty_span.data());

        auto cit = empty_span.cbegin();
        EXPECT_EQ(cit, nullptr);
        EXPECT_EQ(cit, empty_span.data());
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);

        constexpr auto it = span.begin();
        static_assert(it == m_const_array);

        constexpr auto cit = span.cbegin();
        static_assert(cit == m_const_array);
    }
}

TEST_F(SpanTest, test_iterators_end)
{
    {
        // Non-empty span
        core::span<int> span(m_array);

        auto it = span.end();
        EXPECT_EQ(it, m_array + core::size(m_array));
        EXPECT_EQ(it - span.begin(), static_cast<ptrdiff_t>(span.size()));

        auto cit = span.cend();
        EXPECT_EQ(cit, m_array + core::size(m_array));
        EXPECT_EQ(cit - span.cbegin(), static_cast<ptrdiff_t>(span.size()));
        static_assert(std::is_same_v<decltype(cit), core::span<int>::const_iterator>);
    }
    {
        // Empty span
        core::span<int> empty_span {};

        auto it = empty_span.end();
        EXPECT_EQ(it, nullptr);
        EXPECT_EQ(it, empty_span.begin());

        auto cit = empty_span.cend();
        EXPECT_EQ(cit, nullptr);
        EXPECT_EQ(cit, empty_span.cbegin());
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);
        constexpr auto it = span.end();

        static_assert(it == m_const_array + core::size(m_const_array));
    }
}

TEST_F(SpanTest, test_range_based_for_loop)
{
    {
        // Non-empty span
        constexpr core::span<const int> span(m_const_array);
        const auto expected = std::accumulate(m_const_array, m_const_array + core::size(m_const_array), 0);

        int accumulator = 0;
        for (const auto& value : span) {
            accumulator += value;
        }
        EXPECT_EQ(accumulator, expected);
    }
    {
        // Empty span
        core::span<int> empty_span {};

        int accumulator = 0;
        for (const auto& value : empty_span) {
            accumulator += value;
        }
        EXPECT_EQ(accumulator, 0);
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);

        constexpr auto accumulator = [&]() -> int {
            int accumulator = 0;
            for (const auto& value : span) {
                accumulator += value;
            }
            return accumulator;
        }();
        static_assert(accumulator == 60);
    }
}

TEST_F(SpanTest, test_iterator_arithmetic)
{
    constexpr core::span<const int> span(m_const_array);
    constexpr auto begin_it = span.begin();
    constexpr auto end_it = span.end();

    // Test distance
    static_assert(std::is_same_v<decltype(end_it - begin_it), ptrdiff_t>);
    static_assert((end_it - begin_it) == static_cast<ptrdiff_t>(span.size()));

    // Test indexing
    static_assert(*(begin_it + 0) == m_const_array[0]);
    static_assert(*(begin_it + 1) == m_const_array[1]);
    static_assert(*(begin_it + 2) == m_const_array[2]);

    // Test advancement
    constexpr auto advancement = [&]() {
        auto it = begin_it;
        for (size_t i = 0; i < span.size(); ++i) {
            ++it;
        }
        return it;
    }();
    static_assert(advancement == end_it);
}

// Element Access Tests
TEST_F(SpanTest, test_subscript_operator)
{
    constexpr core::span<const int> span(m_const_array);

    static_assert(span[0u] == m_const_array[0]);
    static_assert(span[1u] == m_const_array[1]);
    static_assert(span[2u] == m_const_array[2]);
}

TEST_F(SpanTest, test_data_front_back_accessors)
{
    {
        // Runtime tests
        core::span<int> span(m_array);
        core::span<int> empty_span {};

        // Test data access
        EXPECT_EQ(span.data(), m_array);
        EXPECT_EQ(*span.data(), m_array[0]);
        EXPECT_EQ(empty_span.data(), nullptr);

        // Test front/back access
        EXPECT_EQ(span.front(), m_array[0]);
        EXPECT_EQ(span.back(), m_array[core::size(m_array) - 1]);

        // Test modification through accessors
        *span.data() = 400;
        EXPECT_EQ(span[0u], 400);
        EXPECT_EQ(m_array[0], 400);

        span.front() = 200;
        EXPECT_EQ(span.front(), 200);
        EXPECT_EQ(m_array[0], 200);

        span.back() = 300;
        EXPECT_EQ(span.back(), 300);
        EXPECT_EQ(m_array[core::size(m_array) - 1], 300);
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);

        static_assert(span.data() == m_const_array);
        static_assert(*span.data() == m_const_array[0]);

        static_assert(span.front() == m_const_array[0]);
        static_assert(span.front() == 10);
        static_assert(span.back() == m_const_array[core::size(m_const_array) - 1]);
        static_assert(span.back() == 30);
    }
}

// Observer Functions
TEST_F(SpanTest, test_observers)
{
    constexpr core::span<const int> span(m_const_array);
    constexpr core::span<const int> empty_span {};

    {
        // Size tests
        static_assert(span.size() == core::size(m_const_array));
        static_assert(empty_span.size() == 0u);

        EXPECT_EQ(span.size(), core::size(m_const_array));
        EXPECT_EQ(empty_span.size(), 0u);
    }
    {
        // Size bytes tests
        static_assert(span.size_bytes() == core::size(m_const_array) * sizeof(int));
        static_assert(empty_span.size_bytes() == 0u);

        EXPECT_EQ(span.size_bytes(), core::size(m_const_array) * sizeof(int));
        EXPECT_EQ(empty_span.size_bytes(), 0u);
    }
    {
        // Empty tests
        static_assert(!span.empty());
        static_assert(empty_span.empty());

        EXPECT_FALSE(span.empty());
        EXPECT_TRUE(empty_span.empty());
    }
}

// Subview Operations
TEST_F(SpanTest, test_first_last_subviews)
{
    {
        // Runtime tests
        core::span<int> span(m_array);

        // Test first() with various counts
        auto first_1 = span.first(1u);
        EXPECT_EQ(first_1.size(), 1u);
        EXPECT_EQ(first_1.data(), m_array);
        EXPECT_EQ(first_1[0u], m_array[0]);

        auto first_3 = span.first(3u);
        EXPECT_EQ(first_3.size(), 3u);
        EXPECT_EQ(first_3.data(), m_array);
        for (size_t i = 0; i < 3u; ++i) {
            EXPECT_EQ(first_3[i], m_array[i]);
        }

        auto first_0 = span.first(0u);
        EXPECT_EQ(first_0.size(), 0u);
        EXPECT_EQ(first_0.data(), m_array);
        EXPECT_TRUE(first_0.empty());

        // Test last() with various counts
        auto last_1 = span.last(1u);
        EXPECT_EQ(last_1.size(), 1u);
        EXPECT_EQ(last_1.data(), m_array + 4);
        EXPECT_EQ(last_1[0u], m_array[4]);

        auto last_3 = span.last(3u);
        EXPECT_EQ(last_3.size(), 3u);
        EXPECT_EQ(last_3.data(), m_array + 2);
        for (size_t i = 0; i < 3u; ++i) {
            EXPECT_EQ(last_3[i], m_array[2 + i]);
        }

        auto last_0 = span.last(0u);
        EXPECT_EQ(last_0.size(), 0u);
        EXPECT_EQ(last_0.data(), m_array + 5);
        EXPECT_TRUE(last_0.empty());
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);

        constexpr auto first_1 = span.first(1u);
        static_assert(first_1.size() == 1u);
        static_assert(first_1.data() == m_const_array);
        static_assert(first_1[0u] == m_const_array[0]);

        constexpr auto first_0 = span.first(0u);
        static_assert(first_0.size() == 0u);
        static_assert(first_0.empty());

        constexpr auto last_1 = span.last(1u);
        static_assert(last_1.size() == 1u);
        static_assert(last_1.data() == m_const_array + 2);
        static_assert(last_1[0u] == m_const_array[2]);
        static_assert(last_1[0u] == 30);

        constexpr auto last_0 = span.last(0u);
        static_assert(last_0.size() == 0u);
        static_assert(last_0.empty());
    }
}

TEST_F(SpanTest, test_subspan)
{
    {
        // Runtime tests
        core::span<int> span(m_array);

        // Test subspan with offset only (uses dynamic_extent)
        auto sub_from_2 = span.subspan(2u);
        EXPECT_EQ(sub_from_2.size(), 3u);
        EXPECT_EQ(sub_from_2.data(), m_array + 2);
        for (size_t i = 0; i < 3u; ++i) {
            EXPECT_EQ(sub_from_2[i], m_array[2 + i]);
        }

        // Test subspan with offset and count
        auto sub_2_2 = span.subspan(2u, 2u);
        EXPECT_EQ(sub_2_2.size(), 2u);
        EXPECT_EQ(sub_2_2.data(), m_array + 2);
        EXPECT_EQ(sub_2_2[0u], m_array[2]);
        EXPECT_EQ(sub_2_2[1u], m_array[3]);

        // Test subspan from beginning
        auto sub_0_3 = span.subspan(0u, 3u);
        EXPECT_EQ(sub_0_3.size(), 3u);
        EXPECT_EQ(sub_0_3.data(), m_array);
        for (size_t i = 0; i < 3u; ++i) {
            EXPECT_EQ(sub_0_3[i], m_array[i]);
        }

        // Test empty subspan
        auto sub_empty = span.subspan(2u, 0u);
        EXPECT_EQ(sub_empty.size(), 0u);
        EXPECT_EQ(sub_empty.data(), m_array + 2);
        EXPECT_TRUE(sub_empty.empty());
    }
    {
        // Compile-time tests
        constexpr core::span<const int> span(m_const_array);

        // Test constexpr subspan with offset only
        constexpr auto sub_from_1 = span.subspan(1u);
        static_assert(sub_from_1.size() == 2u);
        static_assert(sub_from_1.data() == m_const_array + 1);
        static_assert(sub_from_1[0u] == m_const_array[1]);
        static_assert(sub_from_1[0u] == 20);

        // Test constexpr subspan with offset and count
        constexpr auto sub_1_1 = span.subspan(1u, 1u);
        static_assert(sub_1_1.size() == 1u);
        static_assert(sub_1_1.data() == m_const_array + 1);
        static_assert(sub_1_1[0u] == 20);

        // Test empty constexpr subspan
        constexpr auto sub_empty = span.subspan(1u, 0u);
        static_assert(sub_empty.size() == 0u);
        static_assert(sub_empty.empty());
    }
}

auto main(int argc, char** argv) -> int
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
