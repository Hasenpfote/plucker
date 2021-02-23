/**
 * @file plucker/plucker_base.h
 */
#pragma once

#include <Eigen/Geometry>

namespace plucker
{

template<typename T>
using Vector3 = Eigen::Matrix<T, 3, 1>;

template<typename T>
using Vector4 = Eigen::Matrix<T, 4, 1>;

template<typename T>
using Vector6 = Eigen::Matrix<T, 6, 1>;

/**
 * Plucker coordinates of a line.
 */
template<typename T>
class Plucker
{
    static_assert(std::is_floating_point<T>::value,
        "Template parameter T must be floating_point type.");
public:
    using value_type = T;

/* Constructors */
    Plucker()
    {}

    explicit Plucker(const Vector6<T>& coord)
        : coord_(coord)
    {}

    Plucker(const Vector3<T>& l, const Vector3<T>& m)
        : Plucker((Vector6<T>() << l, m).finished())
    {}
    /**
     * Creates from distinct homogeneous points on a line,
     * and a line is directed `from -> to`.
     */
    Plucker(const Vector4<T>& from, const Vector4<T>& to)
        : Plucker(
            from.w() * to.head(3) - to.w() * from.head(3),
        	Vector3<T>(from.head(3)).cross(Vector3<T>(to.head(3))))
    {}

/* Accessors */
    Eigen::Ref<const Vector3<T>> l() const noexcept { return coord_.head(3); }
    Eigen::Ref<const Vector3<T>> m() const noexcept { return coord_.tail(3); }

    Eigen::Ref<Vector3<T>> l() noexcept { return coord_.head(3); }
    Eigen::Ref<Vector3<T>> m() noexcept { return coord_.tail(3); }

    const Vector6<T>& coord() const noexcept { return coord_; }
    Vector6<T>& coord() noexcept { return coord_; }

/* Assignment operators */
    Plucker& operator *= (T);

private:
    static constexpr bool needs_to_align = (sizeof(Vector6<T>) % 16) == 0;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(needs_to_align)

private:
    Vector6<T> coord_;
};

/* Assignment operators */

template<typename T>
Plucker<T>&
Plucker<T>::operator *= (T rhs)
{
    coord_ *= rhs;
    return *this;
}

/* Unary operators */

template<typename T>
Plucker<T>
operator + (const Plucker<T>& p)
{
    return p;
}

template<typename T>
Plucker<T>
operator - (const Plucker<T>& p)
{
    return Plucker<T>(-p.coord());
}

/* Binary operators */

template<typename T>
Plucker<T>
operator * (const Plucker<T>& lhs, T rhs)
{
    Plucker<T> temp(lhs);
    return temp *= rhs;
}

template<typename T>
Plucker<T>
operator * (T lhs, const Plucker<T>& rhs)
{
    Plucker<T> temp(rhs);
    return temp *= lhs;
}

/**
 * Computes the reciprocal product of two lines.
 */
template<typename T>
T operator * (const Plucker<T>& lhs, const Plucker<T>& rhs)
{
    return lhs.l().dot(rhs.m()) + rhs.l().dot(lhs.m());
}

}   // namespace plucker