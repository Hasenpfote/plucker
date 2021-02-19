#include <gtest/gtest.h>
#include <plucker/plucker_base.h>
#include <plucker/plucker_geometric.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class PluckerGeometricTest
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
TYPED_TEST_SUITE(PluckerGeometricTest, MyTypes);

TYPED_TEST(PluckerGeometricTest, normalize)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
    const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

    const Plucker line(from, to);

    const auto res = normalize(line);

    EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(1), res.l().norm(), atol);
    EXPECT_ALMOST_EQUAL(TypeParam, line.m().norm() / line.l().norm(), res.m().norm(), atol);
}

TYPED_TEST(PluckerGeometricTest, squared_distance_from_origin_to_line)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
    const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

    const Plucker line(from, to);

    const auto res = squared_distance(line);
    EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(4), res, atol);
}

TYPED_TEST(PluckerGeometricTest, distance_from_origin_to_line)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
    const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

    const Plucker line(from, to);

    const auto res = distance(line);
    EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res, atol);
}

TYPED_TEST(PluckerGeometricTest, distance_of_between_skew_lines)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(0), TypeParam(0), TypeParam(0), TypeParam(1));
        const auto to2 = Vector4(TypeParam(2), TypeParam(0), TypeParam(0), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance_of_between_skew_lines(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res, atol);
    }
    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(0), TypeParam(1));
        const auto to2 = Vector4(TypeParam(2), TypeParam(2), TypeParam(0), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance_of_between_skew_lines(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res, atol);
    }
}

TYPED_TEST(PluckerGeometricTest, distance_of_between_two_parallel_lines)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(2), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to2 = Vector4(TypeParam(2), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance_of_between_two_parallel_lines(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res, atol);
    }
    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance_of_between_two_parallel_lines(line1, line2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res, atol);
    }
}

TYPED_TEST(PluckerGeometricTest, distance_between_two_lines)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(0), TypeParam(0), TypeParam(0), TypeParam(1));
        const auto to2 = Vector4(TypeParam(2), TypeParam(0), TypeParam(0), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance(line1, line2, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res, atol);
    }
    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(0), TypeParam(1));
        const auto to2 = Vector4(TypeParam(2), TypeParam(2), TypeParam(0), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance(line1, line2, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res, atol);
    }
    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(2), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to2 = Vector4(TypeParam(2), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance(line1, line2, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res, atol);
    }
    {
        const auto from1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));
        const auto from2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker line1(from1, to1);
        const Plucker line2(from2, to2);

        const auto res = distance(line1, line2, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res, atol);
    }
}

TYPED_TEST(PluckerGeometricTest, distance_from_point_to_line)
{
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    {
        // A line not passes through the origin.
        const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker line(from, to);
        // A Point is not on a Line.
        const auto point1 = Vector4(TypeParam(0), TypeParam(0), TypeParam(0), TypeParam(1));
        const auto res1 = distance(line, point1, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res1, atol);
        // A Point is on a Line.
        const auto point2 = Vector4(TypeParam(0), TypeParam(2), TypeParam(0), TypeParam(1));
        const auto res2 = distance(line, point2, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res2, atol);
    }
    {
        // A line passes through the origin.
        const auto from = Vector4(TypeParam(0), TypeParam(0), TypeParam(6), TypeParam(1));
        const auto to = Vector4(TypeParam(0), TypeParam(0), TypeParam(4), TypeParam(1));

        const Plucker line(from, to);
        // A Point is not on a Line.
        const auto point1 = Vector4(TypeParam(0), TypeParam(2), TypeParam(0), TypeParam(1));
        const auto res1 = distance(line, point1, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res1, atol);
        // A Point is on a Line.
        const auto point2 = Vector4(TypeParam(0), TypeParam(0), TypeParam(0), TypeParam(1));
        const auto res2 = distance(line, point2, atol);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res2, atol);
    }
}

TYPED_TEST(PluckerGeometricTest, distance_from_point_to_normalized_line)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerGeometricTest<TypeParam>::absolute_tolerance();

    {
        // A line not passes through the origin.
        const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

        const auto line = normalize(Plucker(from, to));
        // A Point is not on a Line.
        const auto point1 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto res1 = distance(line, point1);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res1, atol);
        // A Point is on a Line.
        const auto point2 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto res2 = distance(line, point2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res2, atol);
    }
    {
        // A line passes through the origin.
        const auto from = Vector4(TypeParam(0), TypeParam(0), TypeParam(6), TypeParam(1));
        const auto to = Vector4(TypeParam(0), TypeParam(0), TypeParam(4), TypeParam(1));

        const auto line = normalize(Plucker(from, to));
        // A Point is not on a Line.
        const auto point1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto res1 = distance(line, point1);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(2), res1, atol);
        // A Point is on a Line.
        const auto point2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto res2 = distance(line, point2);
        EXPECT_ALMOST_EQUAL(TypeParam, TypeParam(0), res2, atol);
    }
}

}   // namespace