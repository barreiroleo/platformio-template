#pragma once

#include "types.hpp"

namespace core {

/// Basic std::span-like implementation focused on C-arrays
///
/// Current features:
/// - C-array constructors with automatic size deduction
/// - Iterator-based constructors (pointer + count, iterator range)
/// - Element access (operator[], front, back, data) - unchecked for performance
/// - Iterator support (begin, end, cbegin, cend)
/// - Observers (size, size_bytes, empty)
/// - Subview operations (first, last, subspan) - unchecked for performance
/// - Copy/assignment semantics
/// - Constexpr compatible, no exceptions, no dynamic allocation
///
/// Missing features (future implementation):
/// - std::array constructors (const std::array<U,N>&)
/// - Range-based constructor (R&& r) for C++ containers
/// - std::initializer_list constructor (C++26)
/// - span-to-span conversion constructor
/// - Static/dynamic extent template parameter distinction (span<T,N>)
/// - explicit(extent != dynamic_extent) modifiers
/// - Template constraints for iterator constructors (SFINAE/concepts)
/// - Reverse iterators (rbegin, rend, crbegin, crend)
/// - Comparison operators (==, !=, <, <=, >, >=)
/// - Bounds checking variants (at() method)

/// Special value indicating dynamic extent (C++17 backport of std::dynamic_extent)
/// Used as default parameter in subspan() to indicate "use remaining elements"
/// Value: SIZE_MAX (largest possible size_t value)
inline constexpr core::size_t dynamic_extent = static_cast<core::size_t>(-1);

template <typename T>
class span {
public:
    using element_type = T;
    using value_type = T;
    using size_type = core::size_t;
    using difference_type = core::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = T*;
    using const_iterator = const T*;

    /**
     * @section Constructors and assignment
     **/

    /// @brief Default constructor creates an empty span
    constexpr span() noexcept
        : ptr_(nullptr)
        , size_(0)
    {
    }

    /// @brief Copy constructor
    constexpr span(const span& other) noexcept = default;

    /// @brief Copy assignment
    constexpr span& operator=(const span& other) noexcept = default;

    /// @brief Constructor from pointer and size
    constexpr span(T* ptr, size_type count) noexcept
        : ptr_(ptr)
        , size_(count)
    {
    }

    /// @brief Constructor from C-array with automatic size deduction
    template <size_type N>
    constexpr span(type_identity_t<element_type> (&array)[N]) noexcept
        : ptr_(array)
        , size_(N)
    {
    }

    // TODO(any): Reenable after adding type traits support
    // Prevents ambiguous overload with pointer + count constructor
    /// @brief Iterator-based constructor from pointer + count
    // template <class It>
    // constexpr span(It first, size_type count) noexcept
    //     : ptr_(first)
    //     , size_(count)
    // {
    // }
    // /// @brief Iterator-based constructor from iterator range [first, last)
    // template <class It, class End>
    // constexpr span(It first, End last) noexcept
    //     : ptr_(first)
    //     , size_(last - first)
    // {
    // }

    /**
     * @section Iterators
     **/

    /// @brief Returns iterator to the beginning
    constexpr iterator begin() const noexcept { return ptr_; }

    /// @brief Returns iterator to the end
    constexpr iterator end() const noexcept { return ptr_ + size_; }

    /// @brief Returns const iterator to the beginning
    constexpr const_iterator cbegin() const noexcept { return ptr_; }

    /// @brief Returns const iterator to the end
    constexpr const_iterator cend() const noexcept { return ptr_ + size_; }

    /**
     * @section Element access
     **/

    /// @brief Access first element (unchecked)
    constexpr reference front() const noexcept { return ptr_[0]; }

    /// @brief Access last element (unchecked)
    constexpr reference back() const noexcept { return ptr_[size_ - 1]; }

    /// @brief Access element at index (unchecked)
    /// @Todo: Ensure only unsigned integral types are used as index. Requires type traits support.
    /// static_assert(std::is_unsigned<Index>::value, "Span index must be unsigned to prevent negative indexing");
    /// static_assert(std::is_integral<Index>::value, "Span index must be an integral type");
    constexpr reference operator[](size_type index) const noexcept { return ptr_[index]; }

    /// @brief Direct access to underlying data
    constexpr T* data() const noexcept { return ptr_; }

    /**
     * @section Observers
     **/

    /// @brief Returns number of elements
    constexpr size_type size() const noexcept { return size_; }

    /// @brief Returns size in bytes
    constexpr size_type size_bytes() const noexcept { return size_ * sizeof(T); }

    /// @brief Checks if span is empty
    constexpr bool empty() const noexcept { return size_ == 0; }

    /**
     * @section Subviews
     **/

    /// @brief Returns subspan of first count elements
    constexpr span first(size_type count) const noexcept { return span(ptr_, count); }

    /// @brief Returns subspan of last count elements
    constexpr span last(size_type count) const noexcept { return span(ptr_ + size_ - count, count); }

    /// @brief Returns subspan starting at offset with optional count
    constexpr span subspan(size_type offset, size_type count = dynamic_extent) const noexcept
    {
        return span(ptr_ + offset, count != dynamic_extent ? count : size_ - offset);
    }

private:
    T* ptr_;
    size_type size_;
};

} // namespace core
