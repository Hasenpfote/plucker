/**
 * @file plucker/plucker_common.h
 * @brief This file provides common functions for plucker types.
 */
#pragma once

namespace plucker
{

/**
 * Returns the moment of a line about a line.
 * Note: Needs normalized lines.
 */
template<typename T>
T moment(const Plucker<T>& p1, const Plucker<T>& p2)
{
    return p1 * p2;
}

/**
 * Returns the moment of a line about a point.
 * Note: Needs a normalized line.
 */
template<typename T>
Vector3<T> moment(const Plucker<T>& line, const Vector3<T>& point)
{
    return line.m() - point.cross(line.l());
}

/**
 * Returns the closest point on a line to a point.
 * Note: Needs a normalized line.
 */
template<typename T>
Vector3<T> closest_point(const Plucker<T>& line, const Vector3<T>& point)
{
    const auto m = moment(line, point);
    return point + line.l().cross(m); 
}

/**
 * Returns the closest point on a line to the origin.
 */
template<typename T>
Vector4<T> closest_point(const Plucker<T>& p)
{
    Vector4<T> res;
    res << p.l().cross(p.m()), p.l().squaredNorm();
    return res;
}

/**
 * Returns a point on a line.
 */
template<typename T>
Vector4<T> point_on_line(const Plucker<T>& p, T t)
{
    Vector4<T> res;
    res << p.l().cross(p.m()) + t * p.l(), p.l().squaredNorm();
    return res;
}

}   // namespace plucker