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
        const Plucker res(from.homogeneous().eval(), to.homogeneous().eval());
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
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    auto p = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

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
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto p = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

    const Vector3 l = p.l();
    const Vector3 m = p.m();

    const auto res = +p;
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
}

TYPED_TEST(PluckerBaseTest, UnaryMinus)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto p = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

    const Vector3 l = -p.l();
    const Vector3 m = -p.m();

    const auto res = -p;
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
    EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
}

TYPED_TEST(PluckerBaseTest, Multiplication)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();
    // p1 * p2
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        const auto value = p1.l().dot(p2.m()) + p2.l().dot(p1.m());

        EXPECT_ALMOST_EQUAL(TypeParam, value, p1 * p2, atol);
    }
    // p * scalar
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        const auto s = TypeParam(2);

        const Vector3 l = p.l() * s;
        const Vector3 m = p.m() * s;

        const auto res = p * s;
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
    // scalar * p
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        const auto s = TypeParam(2);

        const Vector3 l = p.l() * s;
        const Vector3 m = p.m() * s;

        const auto res = s * p;
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res.m(), atol);
    }
}

}   // namespace