/**
 * @file plucker/relational.h
 * @brief This file provides relational functions.
 */
#pragma once

#include <cmath>

namespace plucker
{

namespace detail
{

template<typename T>
constexpr T
abs(T x)
{
    return x < static_cast<T>(0) ? -x : x;
}

#if ((defined(_MSVC_LANG) && _MSVC_LANG < 201402L) || __cplusplus < 201402L)
template <class T>
constexpr const T&
max(const T& a, const T& b)
{
    return a < b ? b : a;
}
#endif

template<typename T>
constexpr bool
almost_equal(T lhs, T rhs, T rel_tolerance, T abs_tolerance)
{
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
    return abs(lhs - rhs)
        <= std::max(abs_tolerance, rel_tolerance * std::max(abs(lhs), abs(rhs)));
#else
    return abs(lhs - rhs)
        <= max(abs_tolerance, rel_tolerance * max(abs(lhs), abs(rhs)));
#endif
}

template<typename T>
constexpr bool
almost_equal(T lhs, T rhs, T tolerance)
{
#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
    return abs(lhs - rhs)
        <= tolerance * std::max(T(1), std::max(abs(lhs), abs(rhs)));
#else
    return abs(lhs - rhs)
        <= tolerance * max(T(1), max(abs(lhs), abs(rhs)));
#endif
}

template<typename T>
constexpr bool
almost_zero(T x, T tolerance)
{
    return abs(x) <= tolerance;
}

}   // namespace detail

}   // namespace plucker
