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

int main()
{
    using value_type = double;
    using Plucker = plucker::Plucker<value_type>;

    return 0;
}
```



## References

- [Eigen](http://eigen.tuxfamily.org)



## License

This software is released under the CC0 License, see LICENSE.
