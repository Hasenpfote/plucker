/**
 * @file plucker/mat_relational.h
 * @brief This file provides relational functions for MatrixBase types.
 */
#pragma once

#include <Eigen/Core>

namespace plucker
{

namespace detail
{

/**
 * Returns true if two matrices are element-wise equal within tolerance.
 */
template<typename Derived>
bool almost_equal(
    const Eigen::MatrixBase<Derived>& lhs,
    const Eigen::MatrixBase<Derived>& rhs,
    typename Eigen::MatrixBase<Derived>::RealScalar rel_tolerance,
    typename Eigen::MatrixBase<Derived>::RealScalar abs_tolerance)
{
    return ((lhs - rhs).array().abs()
        <= Eigen::MatrixBase<Derived>::Constant(abs_tolerance).array().max(rel_tolerance * lhs.array().abs().max(rhs.array().abs()))).all();
}

/**
 * Returns true if two matrices are element-wise equal within tolerance.
 */
template<typename Derived>
bool almost_equal(
    const Eigen::MatrixBase<Derived>& lhs,
    const Eigen::MatrixBase<Derived>& rhs,
    typename Eigen::MatrixBase<Derived>::RealScalar tolerance)
{
    return ((lhs - rhs).array().abs()
        <= tolerance * Eigen::MatrixBase<Derived>::Ones().array().max(lhs.array().abs().max(rhs.array().abs()))).all();
}

/**
 * Returns true if a matrix is element-wise equal to zero within tolerance.
 */
template<typename Derived>
bool almost_zero(
    const Eigen::MatrixBase<Derived>& x,
    typename Eigen::MatrixBase<Derived>::RealScalar tolerance)
{
    return (x.array().abs() <= tolerance).all();
}

}   // namespace detail

}   // namespace plucker
