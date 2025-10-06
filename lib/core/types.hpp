#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstddef>
#endif

namespace core {

#ifdef ARDUINO
using size_t = ::size_t;
using ptrdiff_t = ::ptrdiff_t;
#else
using size_t = ::std::size_t;
using ptrdiff_t = ::std::ptrdiff_t;
#endif

/// Helper for type deduction in constructors (C++17 backport of std::type_identity)
/// Prevents unwanted template argument deduction and array-to-pointer decay
/// Allows span<const T> to accept T[N] arrays by preventing exact type matching
template <typename T>
struct type_identity {
    using type = T;
};
template <typename T>
using type_identity_t = typename type_identity<T>::type;

/// @brief Backport of std::size for containers with size() method
template <class Cont>
constexpr auto size(const Cont& c) noexcept(noexcept(c.size())) -> decltype(c.size())
{
    return c.size();
}

/// @brief Backport of std::size for C-style arrays
template <class T, size_t Size>
constexpr size_t size(const T (&)[Size]) noexcept
{
    return Size;
}

} // namespace core
