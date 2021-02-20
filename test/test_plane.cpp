#include <gtest/gtest.h>
#include <Eigen/Geometry>
#include <plucker/plane.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class PlaneTest
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
TYPED_TEST_SUITE(PlaneTest, MyTypes);

TYPED_TEST(PlaneTest, Constructor)
{
    using Plane = plucker::Plane<TypeParam>;
    using Vector4 = typename Plane::Vector4;

    constexpr auto atol = PlaneTest<TypeParam>::absolute_tolerance();

    const auto coord = Vector4(TypeParam(1), TypeParam(2), TypeParam(3), TypeParam(4));

    Plane res1(coord.x(), coord.y(), coord.z(), coord.w());
    Plane res2(coord.head(3), coord.w());
    Plane res3(coord);
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res1.coord(), atol);
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res2.coord(), atol);
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res3.coord(), atol);
}

TYPED_TEST(PlaneTest, Accessor)
{
    using Plane = plucker::Plane<TypeParam>;
    using Vector4 = typename Plane::Vector4;

    constexpr auto atol = PlaneTest<TypeParam>::absolute_tolerance();

    const auto coord = Vector4(TypeParam(1), TypeParam(2), TypeParam(3), TypeParam(4));

    {
        Plane res;
        res.a() = coord.x();
        res.b() = coord.y();
        res.c() = coord.z();
        res.d() = coord.w();
        EXPECT_ALMOST_EQUAL(TypeParam, coord.x(), res.a(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.y(), res.b(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.z(), res.c(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.w(), res.d(), atol);
    }
    {
        const auto n = coord.head(3);

        Plane res;
        res.normal() = n;
        res.d() = coord.w();
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, n, res.normal(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.w(), res.d(), atol);
    }
    {
        Plane res;
        res.coord() = coord;
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res.coord(), atol);
    }
}

}   // namespace