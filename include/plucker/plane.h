/**
 * @file plucker/plane.h
 */
#include <Eigen/Geometry>

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

    Plane(T a, T b, T c, T d)
        : a_(a), b_(b), c_(c), d_(d)
    {}

    Plane(Vector3 n, T d)
        : Plane(n.x(), n.y(), n.z(), d)
    {}

    explicit Plane(Vector4 v)
        : Plane(v.x(), v.y(), v.z(), v.w())
    {}

/* Accessors */
    T a() const noexcept { return a_; }
    T b() const noexcept { return b_; }
    T c() const noexcept { return c_; }
    T d() const noexcept { return d_; }

    T& a() noexcept { return a_; }
    T& b() noexcept { return b_; }
    T& c() noexcept { return c_; }
    T& d() noexcept { return d_; }

    Vector3 normal() const noexcept { return Vector3(a_, b_, c_); }
    Vector4 as_vector4() const noexcept { return Vector4(a_, b_, c_, d_); }

private:
    T a_;
    T b_;
    T c_;
    T d_;
};

}   // namespace plucker