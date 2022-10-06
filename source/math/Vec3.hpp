#ifndef VEC3_HPP
#define VEC3_HPP

#include <ostream>

namespace Kuma3D {

/**
 * A simple 3-dimensional vector class.
 */
class Vec3
{
  public:
    Vec3()
      : x(0.0),
        y(0.0),
        z(0.0) {}

    /**
     * Value constructor. This allows the initialization of each value
     * in the vector.
     *
     * @param a The x-coordinate of the vector.
     * @param b The y-coordinate of the vector.
     * @param c The z-coordinate of the vector.
     */
    Vec3(float a, float b, float c)
      : x(a)
      , y(b)
      , z(c) {};

    Vec3& operator+=(const Vec3& rhs)
    {
      x += rhs.x;
      y += rhs.y;
      z += rhs.z;
      return (*this);
    }

    Vec3& operator-=(const Vec3& rhs)
    {
      x -= rhs.x;
      y -= rhs.y;
      z -= rhs.z;
      return (*this);
    }

    Vec3& operator*=(const Vec3& rhs)
    {
      x *= rhs.x;
      y *= rhs.y;
      z *= rhs.z;
      return (*this);
    }

    Vec3& operator/=(const Vec3& rhs)
    {
      x /= rhs.x;
      y /= rhs.y;
      z /= rhs.z;
      return (*this);
    }

    Vec3& operator*=(float rhs)
    {
      x *= rhs;
      y *= rhs;
      z *= rhs;
      return (*this);
    }

    Vec3& operator/=(float rhs)
    {
      x /= rhs;
      y /= rhs;
      z /= rhs;
      return (*this);
    }

    float x, y, z;
};

/******************************************************************************/
inline std::ostream& operator<<(std::ostream& os, const Vec3& rhs)
{
  os << rhs.x << " " << rhs.y << " " << rhs.z;
  return os;
}

/******************************************************************************/
inline Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
{
  return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
};

/******************************************************************************/
inline Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
{
  return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
};

/******************************************************************************/
inline Vec3 operator*(const Vec3& lhs, float rhs)
{
  return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}

/******************************************************************************/
inline Vec3 operator/(const Vec3& lhs, float rhs)
{
  return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}

} // namespace Kuma3D

#endif
