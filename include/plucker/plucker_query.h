/**
 * @file plucker/plucker_query.h
 * @brief This file provides query functions for plucker types.
 */
#pragma once

#include <cassert>
#include <cmath>
#include "relational.h"
#include "mat_relational.h"
#include "plane.h"

namespace plucker
{

namespace detail
{

/**
 * Returns true if two vectors are perpendicular.
 */
template<typename T>
bool are_perpendicular(const Vector3<T>& v1, const Vector3<T>& v2, T tolerance)
{
    return detail::almost_zero(v1.dot(v2), tolerance);
}

/**
 * Returns true if two vectors are parallel.
 */
template<typename T>
bool are_parallel(const Vector3<T>& v1, const Vector3<T>& v2, T tolerance)
{
    return detail::almost_zero(v1.cross(v2).norm(), tolerance);
}

}   // namespace detail

/**
 * Returns true if a line is at infinity.
 */
template<typename T>
bool is_at_infinity(const Plucker<T>& p, T tolerance)
{
    return detail::almost_zero(p.l(), tolerance);
}

/**
 * Returns true if a line passes through the origin.
 */
template<typename T>
bool passes_through_origin(const Plucker<T>& p, T tolerance)
{
    return detail::almost_zero(p.m(), tolerance);
}

/**
 * Returns true if two lines are same.
 */
template<typename T>
bool are_same(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    if(!detail::are_parallel(p1.l(), p2.l(), tolerance)
    || !detail::are_parallel(p1.m(), p2.m(), tolerance))
        return false;

    if(std::signbit(p1.l().dot(p2.l()))
    != std::signbit(p1.m().dot(p2.m())))
        return false;

    const auto scale1 = p2.l().norm() / p1.l().norm();
    const auto scale2 = p2.m().norm() / p1.m().norm();

    return detail::almost_equal(scale1, scale2, tolerance);
}

/**
 * Returns true if two lines are perpendicular.
 */
template<typename T>
bool are_perpendicular(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    const auto cosine = p1.l().dot(p2.l());
    return detail::almost_zero(cosine, tolerance);
}

/**
 * Returns true if two lines are parallel.
 */
template<typename T>
bool are_parallel(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    return detail::are_parallel(p1.l(), p2.l(), tolerance);
}

/**
 * Returns true if two lines are coplanar.
 */
template<typename T>
bool are_coplanar(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    return detail::almost_zero(p1 * p2, tolerance);
}

/**
 * Returns true if two lines are skew.
 */
template<typename T>
bool are_skew(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    return !are_coplanar(p1, p2, tolerance);
}

/**
 * Returns true if two lines have intersection.
 */
template<typename T>
bool has_intersection(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    return are_coplanar(p1, p2, tolerance) && !are_parallel(p1, p2, tolerance);
}

/**
 * Returns true if a line contains a point.
 */
template<typename T>
bool contains(const Plucker<T> line, const Vector4<T>& point, T tolerance)
{
    const auto v = Vector3<T>(point.head(3)).cross(line.l());
    return detail::almost_equal(v, line.m(), tolerance);
}

/**
 * Returns true if a plane contains a point.
 */
template<typename T>
bool contains(const Plane<T> plane, const Vector4<T>& point, T tolerance)
{
    return detail::almost_zero(plane.coord().dot(point), tolerance);
}

/**
 * Returns true if a plane contains a line.
 */
template<typename T>
bool contains(const Plane<T>& plane, const Plucker<T> line, T tolerance)
{
    if(!detail::are_perpendicular(plane.normal().eval(), line.l(), tolerance))
        return false;

    Vector4<T> point;
    point << line.l().cross(line.m()), line.l().squaredNorm();
    return contains(plane, point, tolerance);
}

}   // namespace plucker