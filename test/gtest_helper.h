#pragma once

#include <gtest/gtest.h>

namespace gtest_helper_detail
{

template<typename T>
bool almost_equal(T lhs, T rhs, T tolerance);

extern template bool almost_equal(float lhs, float rhs, float tolerance);
extern template bool almost_equal(double lhs, double rhs, double tolerance);

template<typename T>
bool not_almost_equal(T lhs, T rhs, T tolerance);

extern template bool not_almost_equal(float lhs, float rhs, float tolerance);
extern template bool not_almost_equal(double lhs, double rhs, double tolerance);

template<typename T>
bool dense_almost_equal(const Eigen::DenseBase<T>& lhs, const Eigen::DenseBase<T>& rhs, T tolerance);

extern template bool dense_almost_equal(const Eigen::DenseBase<float>& lhs, const Eigen::DenseBase<float>& rhs, float tolerance);
extern template bool dense_almost_equal(const Eigen::DenseBase<double>& lhs, const Eigen::DenseBase<double>& rhs, double tolerance);

template<typename T>
bool dense_not_almost_equal(const Eigen::DenseBase<T>& lhs, const Eigen::DenseBase<T>& rhs, T tolerance);

extern template bool dense_not_almost_equal(const Eigen::DenseBase<float>& lhs, const Eigen::DenseBase<float>& rhs, float tolerance);
extern template bool dense_not_almost_equal(const Eigen::DenseBase<double>& lhs, const Eigen::DenseBase<double>& rhs, double tolerance);

template<typename _Scalar, int _Rows, int _Cols>
bool almost_equal(
    const Eigen::Matrix<_Scalar, _Rows, _Cols>& lhs,
    const Eigen::Matrix<_Scalar, _Rows, _Cols>& rhs,
    _Scalar tolerance)
{
    return ((lhs - rhs).array().abs()
        <= tolerance * Eigen::Matrix<_Scalar, _Rows, _Cols>::Ones().array().max(lhs.array().abs().max(rhs.array().abs()))).all();
}

template<typename _Scalar, int _Rows, int _Cols>
bool not_almost_equal(
    const Eigen::Matrix<_Scalar, _Rows, _Cols>& lhs,
    const Eigen::Matrix<_Scalar, _Rows, _Cols>& rhs,
    _Scalar tolerance)
{
    return !almost_equal(lhs, rhs, tolerance);
}

}   // gtest_helper_detail

#define EXPECT_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::almost_equal<type>, val1, val2, tolerance)

#define EXPECT_NOT_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::not_almost_equal<type>, val1, val2, tolerance)

#define EXPECT_VEC3_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3((gtest_helper_detail::almost_equal<type, 3, 1>), val1, val2, tolerance)

#define EXPECT_VEC4_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3((gtest_helper_detail::almost_equal<type, 4, 1>), val1, val2, tolerance)

#define EXPECT_VEC6_ALMOST_EQUAL(type, val1, val2, tolerance) \
    EXPECT_PRED3((gtest_helper_detail::almost_equal<type, 6, 1>), val1, val2, tolerance)
