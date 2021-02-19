[![Build Status](https://travis-ci.com/Hasenpfote/plucker.svg?branch=master)](https://travis-ci.com/Hasenpfote/plucker)

# plucker

---------------------

## About

- Class template for Plücker coordinates of a line.

- Header-only.



## Compatibility

Supports C++ 11 or higher.

| Compiler | Version           | Remarks |
| -------- | ----------------- | ------- |
| gcc      | 5.5.0 or higher.  |         |
| clang    | 7.0.0 or higher.  |         |
| msvc     | 16.5.4 or higher. |         |

**Older versions of compilers might work as well but they are not tested.**



## Dependencies

- [Eigen](http://eigen.tuxfamily.org)

  

## usage

```c++
#include <plucker/plucker.h>

using value_type = double;
using Vector3 = plucker::Vector3<value_type>;
using Vector4 = plucker::Vector4<value_type>;
using Plucker = plucker::Plucker<value_type>;

bool has_intersection(
    const Plucker& ray,
    const Vector4& p1,
    const Vector4& p2,
    const Vector4& p3)
{
    if(ray * Plucker(p1, p2) > 0.0)
        return false;
    if(ray * Plucker(p2, p3) > 0.0)
        return false;
    if(ray * Plucker(p3, p1) > 0.0)
        return false;
    return true;
}

int main()
{
    // Plucker coordinates of a ray.
    const auto from = Vector4(0.0, 1.0, 6.0, 1.0);
    const auto to = Vector4(0.0, 1.0, 4.0, 1.0);
    const auto ray = Plucker(from, to);

    // Points of a triangle.
    // Here, the front facing of a triangle is counterclockwise.
    const auto p1 = Vector4(3.0, 0.0, -2.0, 1.0);
    const auto p2 = Vector4(0.0, 3.0, -2.0, 1.0);
    const auto p3 = Vector4(-3.0, 0.0, -2.0, 1.0);

    // Does a ray have an intersection with a triangle?
    const auto res = has_intersection(ray, p1, p2, p3);

    std::cout << "has_intersection: " << res << std::endl;  // true

    return 0;
}
```



## References

- [Eigen](http://eigen.tuxfamily.org)



## License

This software is released under the CC0 License, see LICENSE.
