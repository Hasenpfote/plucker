#include <utility>
#include <gtest/gtest.h>
#include <plucker/plucker_base.h>
#include <plucker/plucker_find.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class PluckerFindTest
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
TYPED_TEST_SUITE(PluckerFindTest, MyTypes);

TYPED_TEST(PluckerFindTest, find_intersection_of_two_lines)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));

        const Plucker line1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker line2(from2.homogeneous().eval(), to2.homogeneous().eval());

        const auto res = find_intersection(line1, line2, atol);
        const Vector3 intersection = std::get<1>(res).hnormalized();
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, Vector3(TypeParam(0), TypeParam(2), TypeParam(0)), intersection, atol);
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker line1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker line2(from2.homogeneous().eval(), to2.homogeneous().eval());

        const auto res = find_intersection(line1, line2, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_intersection_of_line_and_plane)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;
    using Plane = plucker::Plane<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const Plucker line(from.homogeneous().eval(), to.homogeneous().eval());

    {
        const Plane plane(TypeParam(0), TypeParam(0), TypeParam(1), TypeParam(1));

        const auto res = find_intersection(line, plane, atol);
        const Vector3 intersection = std::get<1>(res).hnormalized();
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, Vector3(TypeParam(0), TypeParam(2), TypeParam(-1)), intersection, atol);
    }
    {
        const Plane plane(TypeParam(1), TypeParam(0), TypeParam(0), TypeParam(0));

        const auto res = find_intersection(line, plane, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_intersection_of_two_planes)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;
    using Plane = plucker::Plane<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(0), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(0), TypeParam(4));
        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

        const auto plane1 = Plane(TypeParam(-1), TypeParam(-1), TypeParam(0), TypeParam(0));
        const auto plane2 = Plane(TypeParam(1), TypeParam(-1), TypeParam(0), TypeParam(0));

        const auto res = find_intersection(plane1, plane2, atol);
        const auto intersection = std::get<1>(res);
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_TRUE(plucker::detail::are_parallel(line.l().eval(), intersection.l().eval(), atol));
        EXPECT_TRUE(plucker::detail::are_parallel(line.m().eval(), intersection.m().eval(), atol));
    }
    {
        const auto plane1 = Plane(TypeParam(-1), TypeParam(0), TypeParam(0), TypeParam(0));
        const auto plane2 = Plane(TypeParam(1), TypeParam(0), TypeParam(0), TypeParam(0));

        const auto res = find_intersection(plane1, plane2, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_closest_points)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const auto line1 = Plucker(from1.homogeneous().eval(), to1.homogeneous().eval());
        const auto line2 = Plucker(from2.homogeneous().eval(), to2.homogeneous().eval());

        const auto res = find_closest_points(line1, line2, atol);
        const Vector3 point1 = std::get<1>(res).hnormalized();
        const Vector3 point2 = std::get<2>(res).hnormalized();
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, Vector3(TypeParam(0), TypeParam(2), TypeParam(0)), point1, atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, Vector3(TypeParam(0), TypeParam(0), TypeParam(0)), point2, atol);
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));

        const auto line1 = Plucker(from1.homogeneous().eval(), to1.homogeneous().eval());
        const auto line2 = Plucker(from2.homogeneous().eval(), to2.homogeneous().eval());

        const auto res = find_closest_points(line1, line2, atol);
        const Vector3 point1 = std::get<1>(res).hnormalized();
        const Vector3 point2 = std::get<2>(res).hnormalized();
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, point1, point2, atol);
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(6));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(4));

        const auto line1 = Plucker(from1.homogeneous().eval(), to1.homogeneous().eval());
        const auto line2 = Plucker(from2.homogeneous().eval(), to2.homogeneous().eval());

        const auto res = find_closest_points(line1, line2, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_origin_plane_through_line)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());
        Vector4 plane;
        plane << line.m(), TypeParam(0);

        const auto res = find_origin_plane_through_line(line, atol);
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, plane, std::get<1>(res).coord(), atol);
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(0), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(0), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

        const auto res = find_origin_plane_through_line(line, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_plane_through_line)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());
        Vector4 plane;
        plane << line.m().cross(line.l()), line.m().squaredNorm();

        const auto res = find_plane_through_line(line, atol);
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, plane, std::get<1>(res).coord(), atol);
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(0), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(0), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

        const auto res = find_plane_through_line(line, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_common_plane_of_line_and_point)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());
        const auto point = Vector4(TypeParam(2), TypeParam(0), TypeParam(0), TypeParam(1));
        const Vector3 p = point.head(3);
        Vector4 plane;
        plane << line.l().cross(p) + point.w() * line.m(), - line.m().dot(p);

        const auto res = find_common_plane(line, point, atol);
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, plane, std::get<1>(res).coord(), atol);
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());
        const auto point = Vector4(TypeParam(0), TypeParam(2), TypeParam(0), TypeParam(1));

        const auto res = find_common_plane(line, point, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

TYPED_TEST(PluckerFindTest, find_common_plane_of_line_and_vector)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerFindTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());
        const auto vector = Vector3(TypeParam(1), TypeParam(0), TypeParam(0));
        Vector4 plane;
        plane << line.l().cross(vector), - line.m().dot(vector);

        const auto res = find_common_plane(line, vector, atol);
        EXPECT_TRUE(std::get<0>(res));
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, plane, std::get<1>(res).coord(), atol);
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto line = Plucker(from.homogeneous().eval(), to.homogeneous().eval());
        const auto vector = Vector3(TypeParam(0), TypeParam(0), TypeParam(1));

        const auto res = find_common_plane(line, vector, atol);
        EXPECT_FALSE(std::get<0>(res));
    }
}

}   // namespace
