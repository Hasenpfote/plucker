/**
 * @file plucker/vec_relational.h
 * @brief This file provides relational functions for vector types.
 */
#pragma once

#include <cmath>
#include "relational.h"

namespace plucker
{

namespace detail
{

template<typename T>
bool almost_equal(const Vector3<T>& lhs, const Vector3<T>& rhs, T tolerance)
{
    return almost_equal(lhs.x(), rhs.x(), tolerance)
        && almost_equal(lhs.y(), rhs.y(), tolerance)
        && almost_equal(lhs.z(), rhs.z(), tolerance);
}

template<typename T>
bool almost_zero(const Vector3<T>& v, T tolerance)
{
    return almost_zero(v.x(), tolerance)
        && almost_zero(v.y(), tolerance)
        && almost_zero(v.z(), tolerance);
}

template<typename T>
bool almost_equal(const Vector4<T>& lhs, const Vector4<T>& rhs, T tolerance)
{
    return almost_equal(lhs.x(), rhs.x(), tolerance)
        && almost_equal(lhs.y(), rhs.y(), tolerance)
        && almost_equal(lhs.z(), rhs.z(), tolerance)
        && almost_equal(lhs.w(), rhs.w(), tolerance);
}

template<typename T>
bool almost_zero(const Vector4<T>& v, T tolerance)
{
    return almost_zero(v.x(), tolerance)
        && almost_zero(v.y(), tolerance)
        && almost_zero(v.z(), tolerance)
        && almost_zero(v.w(), tolerance);
}

}   // namespace detail

}   // namespace plucker