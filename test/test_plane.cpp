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

    Plane res1(coord);
    Plane res2(coord.x(), coord.y(), coord.z(), coord.w());
    Plane res3(coord.head(3), coord.w());
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res1.coord(), atol);
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res2.coord(), atol);
    EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res3.coord(), atol);
}

TYPED_TEST(PlaneTest, Accessor)
{
    using Plane = plucker::Plane<TypeParam>;
    using Vector3 = typename Plane::Vector3;
    using Vector4 = typename Plane::Vector4;

    constexpr auto atol = PlaneTest<TypeParam>::absolute_tolerance();

    const auto coord = Vector4(TypeParam(1), TypeParam(2), TypeParam(3), TypeParam(4));

    {
        const Plane res1(coord);
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res1.coord(), atol);

        Plane res2;
        res2.coord() = coord;
        EXPECT_VEC4_ALMOST_EQUAL(TypeParam, coord, res2.coord(), atol);
    }
    {
        const Plane res1(coord);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.x(), res1.a(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.y(), res1.b(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.z(), res1.c(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.w(), res1.d(), atol);

        Plane res2;
        res2.a() = coord.x();
        res2.b() = coord.y();
        res2.c() = coord.z();
        res2.d() = coord.w();
        EXPECT_ALMOST_EQUAL(TypeParam, coord.x(), res2.a(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.y(), res2.b(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.z(), res2.c(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.w(), res2.d(), atol);
    }
    {
        const Vector3 n = coord.head(3);

        const Plane res1(coord);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, n, res1.normal(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.w(), res1.d(), atol);

        Plane res2;
        res2.normal() = n;
        res2.d() = coord.w();
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, n, res2.normal(), atol);
        EXPECT_ALMOST_EQUAL(TypeParam, coord.w(), res2.d(), atol);
    }
}

TYPED_TEST(PlaneTest, Alignment)
{
    using Plane = plucker::Plane<TypeParam>;
    using Vector4 = typename Plane::Vector4;

    if((sizeof(Vector4) % 16) != 0)
        GTEST_SKIP_("It does not need to be aligned.");

    {
        Plane* p = new Plane;
        const auto addr = reinterpret_cast<std::uintptr_t>(&(p->coord()));
        EXPECT_TRUE((addr % 16) == 0);
        delete p;
    }
    {
        Plane* p = new Plane[2];
        const auto addr0 = reinterpret_cast<std::uintptr_t>(&(p[0].coord()));
        const auto addr1 = reinterpret_cast<std::uintptr_t>(&(p[1].coord()));
        EXPECT_TRUE((addr0 % 16) == 0);
        EXPECT_TRUE((addr1 % 16) == 0);
        delete[] p;
    }
}

}   // namespace
