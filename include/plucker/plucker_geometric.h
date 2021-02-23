/**
 * @file plucker/plucker_geometric.h
 * @brief This file provides geometric functions for plucker types.
 */
#pragma once

#include <cmath>
#include "plucker_query.h"

namespace plucker
{

/**
 * Returns a normalized line.
 */
template<typename T>
Plucker<T> normalize(const Plucker<T>& p)
{
    return Plucker<T>(p.coord() / p.l().norm());
}

/**
 * Returns the squared distance from the origin to a line.
 */
template<typename T>
T squared_distance(const Plucker<T>& p)
{
    return p.m().squaredNorm() / p.l().squaredNorm();
}

/**
 * Returns the distance from the origin to a line.
 */
template<typename T>
T distance(const Plucker<T>& p)
{
    return std::sqrt(squared_distance(p));
}

/**
 * Returns the distance of between skew lines.
 */
template<typename T>
T distance_of_between_skew_lines(const Plucker<T>& p1, const Plucker<T>& p2)
{
    return std::abs(p1 * p2) / p1.l().cross(p2.l()).norm();
}

/**
 * Returns the distance of between two parallel lines.
 */
template<typename T>
T distance_of_between_two_parallel_lines(const Plucker<T>& p1, const Plucker<T>& p2)
{
    const auto s = (p1.l().dot(p2.l()) < static_cast<T>(0)) ?
         - p2.l().norm() / p1.l().norm() : p2.l().norm() / p1.l().norm();
    return p1.l().cross(p1.m() - p2.m() / s).norm() / p1.l().squaredNorm();
}

/**
 * Returns the shortest distance between two lines.
 */
template<typename T>
T distance(const Plucker<T>& p1, const Plucker<T>& p2, T tolerance)
{
    return are_parallel(p1, p2, tolerance) ?
        distance_of_between_two_parallel_lines(p1, p2) : distance_of_between_skew_lines(p1, p2);
}

/**
 * Returns the shortest distance from a point to a line.
 */
template<typename T>
T distance(const Plucker<T>& line, const Vector4<T>& point, T tolerance)
{
    if(contains(line, point, tolerance))
        return static_cast<T>(0);

    const Vector3<T> p = point.head(3);
    const Vector3<T> n1 = line.l().cross(p) + point.w() * line.m();
    const Vector3<T> n2 = line.l().cross(n1);
    const auto d2 = - line.m().dot(n1);
    return std::abs(n2.dot((p)) + d2) / n2.norm();
}

/**
 * Returns the shortest distance from a point to a line.
 * Note: Needs a normalized line.
 */
template<typename T>
T distance(const Plucker<T>& line, const Vector3<T>& point)
{
    const Vector3<T> moment = line.m() - point.cross(line.l());
    const Vector3<T> p_perp = point + line.l().cross(moment);
    return (p_perp - point).norm();
}

}   // namespace plucker
