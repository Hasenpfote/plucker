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

template<typename DerivedA, typename DerivedB>
bool almost_equal(
    const Eigen::MatrixBase<DerivedA>& lhs,
    const Eigen::MatrixBase<DerivedB>& rhs,
    const typename DerivedA::RealScalar& tolerance = Eigen::NumTraits<typename DerivedA::Scalar>::dummy_precision())
{
    return ((lhs - rhs).array().abs()
        <= tolerance * Eigen::MatrixBase<DerivedA>::Ones().array().max(lhs.array().abs().max(rhs.array().abs()))).all();
}

template<typename DerivedA, typename DerivedB>
bool not_almost_equal(
    const Eigen::MatrixBase<DerivedA>& lhs,
    const Eigen::MatrixBase<DerivedB>& rhs,
    const typename DerivedA::RealScalar& tolerance = Eigen::NumTraits<typename DerivedA::Scalar>::dummy_precision())
{
    return !almost_equal(lhs, rhs, tolerance);
}

template<typename DerivedA, typename DerivedB>
::testing::AssertionResult
assert_mat_almost_equal_pred3_format(
    const char* e1,
    const char* e2,
    const char* e3,
    const Eigen::MatrixBase<DerivedA>& v1,
    const Eigen::MatrixBase<DerivedB>& v2,
    const typename Eigen::MatrixBase<DerivedA>::RealScalar& v3)
{
    if(almost_equal(v1, v2, v3))
        return ::testing::AssertionSuccess();

    return ::testing::AssertionFailure()
        << "pred_text" << "(" << e1 << ", " << e2 << ", " << e3
        << ") evaluates to false, where"
        << "\n"
        << e1 << " evaluates to " << ::testing::PrintToString(v1) << "\n"
        << e2 << " evaluates to " << ::testing::PrintToString(v2) << "\n"
        << e3 << " evaluates to " << ::testing::PrintToString(v3);
}

}   // gtest_helper_detail

#define EXPECT_ALMOST_EQUAL(type, lhs, rhs, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::almost_equal<type>, lhs, rhs, tolerance)

#define EXPECT_NOT_ALMOST_EQUAL(type, lhs, rhs, tolerance) \
    EXPECT_PRED3(gtest_helper_detail::not_almost_equal<type>, lhs, rhs, tolerance)

#define EXPECT_MAT_ALMOST_EQUAL(lhs, rhs, tolerance) \
    EXPECT_PRED_FORMAT3(gtest_helper_detail::assert_mat_almost_equal_pred3_format, lhs, rhs, tolerance)

#define EXPECT_QUAT_ALMOST_EQUAL(lhs, rhs, tolerance) \
    EXPECT_MAT_ALMOST_EQUAL(lhs.coeffs(), rhs.coeffs(), tolerance)

#define EXPECT_VEC3_ALMOST_EQUAL(type, lhs, rhs, tolerance) \
    EXPECT_MAT_ALMOST_EQUAL(lhs, rhs, tolerance)

#define EXPECT_VEC4_ALMOST_EQUAL(type, lhs, rhs, tolerance) \
    EXPECT_MAT_ALMOST_EQUAL(lhs, rhs, tolerance)

#define EXPECT_VEC6_ALMOST_EQUAL(type, lhs, rhs, tolerance) \
    EXPECT_MAT_ALMOST_EQUAL(lhs, rhs, tolerance)
