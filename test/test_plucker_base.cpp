#include <gtest/gtest.h>
#include <plucker/plucker_base.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class PluckerBaseTest
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
TYPED_TEST_SUITE(PluckerBaseTest, MyTypes);

TYPED_TEST(PluckerBaseTest, Accessor)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto l = to - from;
    const auto m = from.cross(to);

    {
        const Plucker res(l, m);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
    {
        const Plucker res(
            static_cast<Vector4>(from.homogeneous()),
            static_cast<Vector4>(to.homogeneous())
        );
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
    {
        Plucker res;
        res.l() = l;
        res.m() = m;
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
}

TYPED_TEST(PluckerBaseTest, MultiplicationAssignment)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
    const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

    auto p = Plucker(from, to);

    const auto s = TypeParam(2);

    const Vector3 l = p.l() * s;
    const Vector3 m = p.m() * s;

    p *= s;

    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, p.l(), atol);
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, p.m(), atol);
}

TYPED_TEST(PluckerBaseTest, UnaryPlus)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
    const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

    const auto p = Plucker(from, to);

    const Vector3 l = p.l();
    const Vector3 m = p.m();

    const auto res = +p;
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
}

TYPED_TEST(PluckerBaseTest, UnaryMinus)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
    const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

    const auto p = Plucker(from, to);

    const Vector3 l = -p.l();
    const Vector3 m = -p.m();

    const auto res = -p;
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
}

TYPED_TEST(PluckerBaseTest, Multiplication)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();
    // p1 * p2
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(
            static_cast<Vector4>(from1.homogeneous()),
            static_cast<Vector4>(to1.homogeneous())
        );
        const Plucker p2(
            static_cast<Vector4>(from2.homogeneous()),
            static_cast<Vector4>(to2.homogeneous())
        );

        const auto value = p1.l().dot(p2.m()) + p2.l().dot(p1.m());

        EXPECT_ALMOST_EQUAL(TypeParam, value, p1 * p2, atol);
    }
    // p * scalar
    {
        const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker p(from, to);

        const auto s = TypeParam(2);

        const Vector3 l = p.l() * s;
        const Vector3 m = p.m() * s;

        const auto res = p * s;
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
    // scalar * p
    {
        const auto from = Vector4(TypeParam(0), TypeParam(2), TypeParam(6), TypeParam(1));
        const auto to = Vector4(TypeParam(0), TypeParam(2), TypeParam(4), TypeParam(1));

        const Plucker p(from, to);

        const auto s = TypeParam(2);

        const Vector3 l = p.l() * s;
        const Vector3 m = p.m() * s;

        const auto res = s * p;
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
}

}   // namespace