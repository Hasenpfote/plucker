/**
 * @file plucker/plane.h
 */
#include <Eigen/Core>

namespace plucker
{

template<typename T>
class Plane
{
    static_assert(std::is_floating_point<T>::value,
        "Template parameter T must be floating_point type.");
public:
    using value_type = T;
    using Vector3 = Eigen::Matrix<T, 3, 1>;
    using Vector4 = Eigen::Matrix<T, 4, 1>;

/* Constructors */
    Plane()
    {}

    explicit Plane(const Vector4& coord)
        : coord_(coord)
    {}

    Plane(T a, T b, T c, T d)
        : Plane((Vector4() << a, b, c, d).finished())
    {}

    Plane(const Vector3& n, T d)
        : Plane((Vector4() << n, d).finished())
    {}

/* Accessors */
    T a() const noexcept { return coord_.x(); }
    T b() const noexcept { return coord_.y(); }
    T c() const noexcept { return coord_.z(); }
    T d() const noexcept { return coord_.w(); }

    T& a() noexcept { return coord_.x(); }
    T& b() noexcept { return coord_.y(); }
    T& c() noexcept { return coord_.z(); }
    T& d() noexcept { return coord_.w(); }

    Eigen::Ref<const Vector3> normal() const noexcept { return coord_.head(3); }
    Eigen::Ref<Vector3> normal() noexcept { return coord_.head(3); }

    const Vector4& coord() const noexcept { return coord_; }
    Vector4& coord() noexcept { return coord_; }

private:
    static constexpr bool needs_to_align = (sizeof(Vector4) % 16) == 0;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW_IF(needs_to_align)

private:
    Vector4 coord_;
};

}   // namespace plucker
