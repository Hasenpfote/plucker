/**
 * @file plucker/plucker_find.h
 * @brief This file provides find functions for plucker types.
 */
#pragma once

#include <tuple>
#include "plucker_query.h"

namespace plucker
{

/**
 * Returns the intersection of two lines.
 */
template<typename T>
std::tuple<bool, Vector4<T>>
find_intersection(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    if(!are_coplanar(p1, p2, tolerance) || are_parallel(p1, p2, tolerance))
        return std::make_tuple(false, Vector4<T>());

    const auto n = p1.l().cross(p2.l());

    Vector4<T> point;
    point << - p1.m().cross(p2.l().cross(n)) + p2.m().dot(n) * p1.l(), n.squaredNorm();

    return std::make_tuple(true, point);
}

/**
 * Returns the intersection of a line and a plane.
 */
template<typename T>
std::tuple<bool, Vector4<T>>
find_intersection(const Plucker<T>& line, const Plane<T>& plane, T tolerance)
{
    const Vector3<T> n = plane.normal();
    if(detail::are_perpendicular(line.l().eval(), n, tolerance))
        return std::make_tuple(false, Vector4<T>());

    Vector4<T> point;
    point << n.cross(line.m()) - plane.d() * line.l(), line.l().dot(n);

    return std::make_tuple(true, point);
}

/**
 * Returns the intersection of two planes.
 */
template<typename T>
std::tuple<bool, Plucker<T>>
find_intersection(const Plane<T>& plane1, const Plane<T>& plane2, T tolerance)
{
    const Vector3<T> n1 = plane1.normal();
    const Vector3<T> n2 = plane2.normal();
    if(detail::are_parallel(n1, n2, tolerance))
        return std::make_tuple(false, Plucker<T>());

    return std::make_tuple(true, Plucker<T>(n1.cross(n2), plane2.d() * n1 - plane1.d() * n2));
}

/**
 * Returns points on two skew lines closest to one another.
 */
template<typename T>
std::tuple<bool, Vector4<T>, Vector4<T>>
find_closest_points(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    if(are_parallel(p1, p2, tolerance))
        return std::make_tuple(false, Vector4<T>(), Vector4<T>());

    const auto n = p1.l().cross(p2.l());
    const auto w = n.squaredNorm();

    Vector4<T> point1;
    point1 << - p1.m().cross(p2.l().cross(n)) + p2.m().dot(n) * p1.l(), w;

    Vector4<T> point2;
    point2 << p2.m().cross(p1.l().cross(n)) - p1.m().dot(n) * p2.l(), w;

    return std::make_tuple(true, point1, point2);
}

/**
 * Returns an origin plane through a line.
 */
template<typename T>
std::tuple<bool, Plane<T>>
find_origin_plane_through_line(const Plucker<T>& p, T tolerance)
{
    if(passes_through_origin(p, tolerance))
        return std::make_tuple(false, Plane<T>());

    return std::make_tuple(true, Plane<T>(p.m(), static_cast<T>(0)));
}

/**
 * Returns a plane through a line.
 */
template<typename T>
std::tuple<bool, Plane<T>>
find_plane_through_line(const Plucker<T>& p, T tolerance)
{
    if(passes_through_origin(p, tolerance))
        return std::make_tuple(false, Plane<T>());

    return std::make_tuple(true, Plane<T>(p.m().cross(p.l()), p.m().squaredNorm()));
}

/**
 * Returns the common plane of a line and a point.
 * e.g. (P:w) is a point.
 */
template<typename T>
std::tuple<bool, Plane<T>>
find_common_plane(const Plucker<T>& line, const Vector4<T>& point, T tolerance)
{
    if(contains(line, point, tolerance))
        return std::make_tuple(false, Plane<T>());

    const Vector3<T> p = point.head(3);

    Vector4<T> plane;
    plane << line.l().cross(p) + point.w() * line.m(), - line.m().dot(p);

    return std::make_tuple(true, Plane<T>(plane));
}

/**
 * Returns the common plane of a line and a vector.
 */
template<typename T>
std::tuple<bool, Plane<T>>
find_common_plane(const Plucker<T>& line, const Vector3<T>& vector, T tolerance)
{
    if(detail::are_parallel(line.l().eval(), vector, tolerance))
        return std::make_tuple(false, Plane<T>());

    Vector4<T> plane;
    plane << line.l().cross(vector), - line.m().dot(vector);

    return std::make_tuple(true, Plane<T>(plane));
}

}   // namespace plucker