#include <gtest/gtest.h>
#include <plucker/plucker_base.h>
#include <plucker/plucker_query.h>
#include "gtest_helper.h"

namespace
{

template<typename T>
class PluckerQueryTest
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
TYPED_TEST_SUITE(PluckerQueryTest, MyTypes);

TYPED_TEST(PluckerQueryTest, is_at_infinity)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        EXPECT_TRUE(is_at_infinity(p, atol));
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        EXPECT_FALSE(is_at_infinity(p, atol));
    }
}

TYPED_TEST(PluckerQueryTest, passes_through_origin)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(0), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(0), TypeParam(4));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        EXPECT_TRUE(passes_through_origin(p, atol));
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const Plucker p(from.homogeneous().eval(), to.homogeneous().eval());

        EXPECT_FALSE(passes_through_origin(p, atol));
    }
}

TYPED_TEST(PluckerQueryTest, are_same)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        EXPECT_TRUE(
            are_same(
                Plucker(from.homogeneous().eval(), to.homogeneous().eval()),
                Plucker(from.homogeneous().eval(), to.homogeneous().eval()),
                atol
            )
        );
        EXPECT_TRUE(
            are_same(
                Plucker(from.homogeneous().eval(), to.homogeneous().eval()),
                Plucker(to.homogeneous().eval(), from.homogeneous().eval()),
                atol
            )
        );
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));

        EXPECT_FALSE(
            are_same(
                Plucker(from1.homogeneous().eval(), to1.homogeneous().eval()),
                Plucker(from2.homogeneous().eval(), to2.homogeneous().eval()),
                atol
            )
        );
    }
    {
        const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

        const auto l = to - from;
        const auto m = from.cross(l);

        EXPECT_TRUE(
            are_same(
                Plucker(l, m),
                Plucker((TypeParam(2) * l).eval(), (TypeParam(2) * m).eval()),
                atol
            )
        );
        EXPECT_TRUE(
            are_same(
                Plucker(l, m),
                Plucker((TypeParam(-2) * l).eval(), (TypeParam(-2) * m).eval()),
                atol
            )
        );
        EXPECT_FALSE(
            are_same(
                Plucker(l, m),
                Plucker((TypeParam(2) * l).eval(), (TypeParam(-2) * m).eval()),
                atol
            )
        );
        EXPECT_FALSE(
            are_same(
                Plucker(l, m),
                Plucker((TypeParam(2) * l).eval(), (TypeParam(3) * m).eval()),
                atol
            )
        );
    }
}

TYPED_TEST(PluckerQueryTest, are_perpendicular)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_TRUE(are_perpendicular(p1, p2, atol));
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(6));
        const auto to2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(4));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_FALSE(are_perpendicular(p1, p2, atol));
    }
}

TYPED_TEST(PluckerQueryTest, are_parallel)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(6));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(4));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_TRUE(are_parallel(p1, p2, atol));
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_FALSE(are_parallel(p1, p2, atol));
    }
}

TYPED_TEST(PluckerQueryTest, are_coplanar)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(6));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(4));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_TRUE(are_coplanar(p1, p2, atol));
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_FALSE(are_coplanar(p1, p2, atol));
    }
}

TYPED_TEST(PluckerQueryTest, are_skew)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_TRUE(are_skew(p1, p2, atol));
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(6));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(4));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_FALSE(are_skew(p1, p2, atol));
    }
}

TYPED_TEST(PluckerQueryTest, has_intersection)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(2), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(2), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_TRUE(has_intersection(p1, p2, atol));
    }
    {
        const auto from1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
        const auto to1 = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));
        const auto from2 = Vector3(TypeParam(0), TypeParam(0), TypeParam(0));
        const auto to2 = Vector3(TypeParam(2), TypeParam(0), TypeParam(0));

        const Plucker p1(from1.homogeneous().eval(), to1.homogeneous().eval());
        const Plucker p2(from2.homogeneous().eval(), to2.homogeneous().eval());

        EXPECT_FALSE(has_intersection(p1, p2, atol));
    }
}

TYPED_TEST(PluckerQueryTest, line_contains_point)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Vector4 = plucker::Vector4<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const Plucker line(from.homogeneous().eval(), to.homogeneous().eval());

    {
        const auto point = Vector4(TypeParam(0), TypeParam(2), TypeParam(0), TypeParam(1));
        EXPECT_TRUE(contains(line, point, atol));
    }
    {
        const auto point = Vector4(TypeParam(0), TypeParam(1), TypeParam(0), TypeParam(1));
        EXPECT_FALSE(contains(line, point, atol));
    }
}

TYPED_TEST(PluckerQueryTest, plane_contains_line)
{
    using Vector3 = plucker::Vector3<TypeParam>;
    using Plucker = plucker::Plucker<TypeParam>;
    using Plane = plucker::Plane<TypeParam>;

    constexpr auto atol = PluckerQueryTest<TypeParam>::absolute_tolerance();

    const auto from = Vector3(TypeParam(0), TypeParam(2), TypeParam(6));
    const auto to = Vector3(TypeParam(0), TypeParam(2), TypeParam(4));

    const Plucker line(from.homogeneous().eval(), to.homogeneous().eval());

    {
        const Plane plane(TypeParam(0), TypeParam(1), TypeParam(0), TypeParam(-2));
        EXPECT_TRUE(contains(plane, line, atol));
    }
    {
        const Plane plane(TypeParam(0), TypeParam(1), TypeParam(0), TypeParam(-3));
        EXPECT_FALSE(contains(plane, line, atol));
    }
}

}   // namespace
