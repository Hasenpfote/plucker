#include <gtest/gtest.h>
#include <plucker/plucker_base.h>
#include <plucker/plucker_common.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class PluckerCommonTest
    : public ::testing::Test
{
protected:
    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, float>::value, U>::type
    absolute_tolerance(){ return 1e-4f; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, double>::value, U>::type
    absolute_tolerance(){ return 1e-8; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, float>::value, U>::type
    relative_tolerance(){ return 1e-5f; }

    template<typename U = T>
    static constexpr typename std::enable_if<std::is_same<U, double>::value, U>::type
    relative_tolerance(){ return 1e-5; }
};

using MyTypes = ::testing::Types<float, double>;
TYPED_TEST_SUITE(PluckerCommonTest, MyTypes);

TYPED_TEST(PluckerCommonTest, moment_of_line_about_line)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerCommonTest<TypeParam>::absolute_tolerance();
    // About two parallel lines.
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const auto l1 = (to1 - from1).normalized();
        const auto l2 = (to2 - from2).normalized();

        const auto line1 = Plucker(l1, from1.cross(l1));
        const auto line2 = Plucker(l2, from2.cross(l2));

        const auto res = moment(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, line1 * line2, res, atol);
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));

        const auto l1 = (to1 - from1).normalized();
        const auto l2 = (to2 - from2).normalized();

        const auto line1 = Plucker(l1, from1.cross(l1));
        const auto line2 = Plucker(l2, from2.cross(l2));

        const auto res = moment(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res, atol);
    }
    // About skew lines.
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(6));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(4));

        const auto l1 = (to1 - from1).normalized();
        const auto l2 = (to2 - from2).normalized();

        const auto line1 = Plucker(l1, from1.cross(l1));
        const auto line2 = Plucker(l2, from2.cross(l2));

        const auto res = moment(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res, atol);
    }
}

TYPED_TEST(PluckerCommonTest, moment_of_line_about_point)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerCommonTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
    const auto l = (to - from).normalized();
    const auto line = Plucker(l, from.cross(l));
    // For any point except the origin.
    {
        const auto point = Vector3(TypeParam(0), TypeParam(1), TypeParam(0));

        const auto res = moment(line, point);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, line.m() - point.cross(line.l()), res, atol);
    }
    // About the origin.
    {
        const auto point = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));

        const auto res = moment(line, point);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, line.m(), res, atol);
    }
    // For the case where a point is on a line.
    {
        const auto point = Vector3(TypeParam(0), TypeParam(2), TypeParam(2));

        const auto res = moment(line, point);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, Vector3(TypeParam(0), TypeParam(0), TypeParam(0)), res, atol) << res;
    }
}

TYPED_TEST(PluckerCommonTest, closest_point_on_line_to_origin)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerCommonTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
    const auto l = (to - from).normalized();
    const auto line = Plucker(l, from.cross(l));
    // For any point except the origin.
    {
        const auto point = Vector3(TypeParam(0), TypeParam(1), TypeParam(0));

        const auto res = closest_point(line, point);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, point + line.l().cross(moment(line, point)), res, atol);
    }
    // About the origin.
    {
        const auto point = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));

        const auto res = closest_point(line, point);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, point + line.l().cross(line.m()), res, atol);
    }
    // For the case where a point is on a line.
    {
        const auto point = Vector3(TypeParam(0), TypeParam(2), TypeParam(2));

        const auto res = closest_point(line, point);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, point, res, atol);
    }
}

TYPED_TEST(PluckerCommonTest, point_on_line)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerCommonTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
    const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

    const auto t = TypeParam(0.5);
    Vector4 expected;
    expected << line.l().cross(line.m()) + t * line.l(), line.l().squaredNorm();

    const auto res = point_on_line(line, t);
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, expected, res, atol);
}

}   // namespace
