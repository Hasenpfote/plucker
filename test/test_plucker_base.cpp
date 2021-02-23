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

TYPED_TEST(PluckerBaseTest, Constructor)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector6 = plucker::Vector6<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto l = to - from;
    const auto m = from.cross(to);

    const auto coord((Vector6() << l, m).finished());

    const Plucker res1(coord);
    const Plucker res2(l, m);
    const Plucker res3(from.homogeneous().eval(), to.homogeneous().eval());

    EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res1.coord(), atol);
    EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res2.coord(), atol);
    EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res3.coord(), atol);
}

TYPED_TEST(PluckerBaseTest, Accessor)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector6 = plucker::Vector6<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto l = to - from;
    const auto m = from.cross(to);

    {
        const Vector6 coord((Vector6() << l, m).finished());

        const Plucker res1(coord);
        EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res1.coord(), atol);

        Plucker res2;
        res2.coord() = coord;
        EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res1.coord(), atol);
    }
    {
        const Plucker res1(l, m);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res1.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res1.m(), atol);

        Plucker res2;
        res2.l() = l;
        res2.m() = m;
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, l, res2.l(), atol);
        EXPECT_VEC3_ALMOST_EQUAL(TypeParam, m, res2.m(), atol);
    }
}

TYPED_TEST(PluckerBaseTest, MultiplicationAssignment)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector6 = plucker::Vector6<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    auto p = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

    const auto s = TypeParam(2);

    const Vector6 coord = p.coord() * s;

    p *= s;

    EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, p.coord(), atol);
}

TYPED_TEST(PluckerBaseTest, UnaryPlus)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector6 = plucker::Vector6<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto p = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

    const Vector6 coord = p.coord();

    const auto res = +p;
    EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res.coord(), atol);
}

TYPED_TEST(PluckerBaseTest, UnaryMinus)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector6 = plucker::Vector6<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerBaseTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const auto p = Plucker(from.homogeneous().eval(), to.homogeneous().eval());

    const Vector6 coord = -p.coord();

    const auto res = -p;
    EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res.coord(), atol);
}

TYPED_TEST(PluckerBaseTest, Multiplication)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector6 = plucker::Vector6<TypeParam>;
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

        const Vector6 coord = p.coord() * s;

        const auto res = p * s;
        EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res.coord(), atol);
    }
    // scalar * p
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        const auto s = TypeParam(2);

        const Vector6 coord = p.coord() * s;

        const auto res = s * p;
        EXPECT_VEC6_ALMOST_EQUAL(TypeParam, coord, res.coord(), atol);
    }
}

TYPED_TEST(PluckerBaseTest, Alignment)
{
    using Vector6 = plucker::Vector6<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    if((sizeof(Vector6) % 16) != 0)
        GTEST_SKIP_("It does not need to be aligned.");

    {
        Plucker* p = new Plucker;
        const auto addr = reinterpret_cast<std::uintptr_t>(&(p->coord()));
        EXPECT_TRUE((addr % 16) == 0);
        delete p;
    }
    {
        Plucker* p = new Plucker[2];
        const auto addr0 = reinterpret_cast<std::uintptr_t>(&(p[0].coord()));
        const auto addr1 = reinterpret_cast<std::uintptr_t>(&(p[1].coord()));
        EXPECT_TRUE((addr0 % 16) == 0);
        EXPECT_TRUE((addr1 % 16) == 0);
        delete[] p;
    }
}

}   // namespace