#ifndef MATRIX4D_HPP
#define MATRIX4D_HPP

#include <ostream>

#include "Vec3.hpp"

namespace Kuma3D {

/**
 * A 4x4 matrix used for vector transformation purposes. The values of the
 * matrix are stored in column-major order.
 */
class Mat4
{
  public:

    /**
     * Default constructor. This initializes the matrix to be an
     * identity matrix.
     */
    Mat4()
    {
      data[0][0] = 1.0; data[1][0] = 0.0; data[2][0] = 0.0; data[3][0] = 0.0;
      data[0][1] = 0.0; data[1][1] = 1.0; data[2][1] = 0.0; data[3][1] = 0.0;
      data[0][2] = 0.0; data[1][2] = 0.0; data[2][2] = 1.0; data[3][2] = 0.0;
      data[0][3] = 0.0; data[1][3] = 0.0; data[2][3] = 0.0; data[3][3] = 1.0;
    };

    /**
     * Value constructor. This allows the initialization of each value
     * in the matrix.
     *
     * @params The values of the matrix, given in column-major order.
     */
    Mat4(float n00, float n10, float n20, float n30,
         float n01, float n11, float n21, float n31,
         float n02, float n12, float n22, float n32,
         float n03, float n13, float n23, float n33)
    {
      data[0][0] = n00; data[1][0] = n10; data[2][0] = n20; data[3][0] = n30;
      data[0][1] = n01; data[1][1] = n11; data[2][1] = n21; data[3][1] = n31;
      data[0][2] = n02; data[1][2] = n12; data[2][2] = n22; data[3][2] = n32;
      data[0][3] = n03; data[1][3] = n13; data[2][3] = n23; data[3][3] = n33;
    };

    float& operator()(const unsigned int& c,
                      const unsigned int& r)
    {
      return data[c][r];
    };

    const float& operator()(const unsigned int& c,
                            const unsigned int& r) const
    {
      return data[c][r];
    };

    float data[4][4];
};

/******************************************************************************/
inline std::ostream& operator<<(std::ostream& os, const Mat4& rhs)
{
  for(int r = 0; r < 4; ++r)
  {
    for(int c = 0; c < 4; ++c)
    {
      os << rhs(c, r) << " ";
    }
    os << "\n";
  }

  return os;
}

/******************************************************************************/
inline Mat4 operator*(const Mat4& lhs, const Mat4& rhs)
{
  return Mat4(lhs(0, 0) * rhs(0, 0) + lhs(1, 0) * rhs(0, 1) + lhs(2, 0) * rhs(0, 2) + lhs(3, 0) * rhs(0, 3),
              lhs(0, 0) * rhs(1, 0) + lhs(1, 0) * rhs(1, 1) + lhs(2, 0) * rhs(1, 2) + lhs(3, 0) * rhs(1, 3),
              lhs(0, 0) * rhs(2, 0) + lhs(1, 0) * rhs(2, 1) + lhs(2, 0) * rhs(2, 2) + lhs(3, 0) * rhs(2, 3),
              lhs(0, 0) * rhs(3, 0) + lhs(1, 0) * rhs(3, 1) + lhs(2, 0) * rhs(3, 2) + lhs(3, 0) * rhs(3, 3),
              lhs(0, 1) * rhs(0, 0) + lhs(1, 1) * rhs(0, 1) + lhs(2, 1) * rhs(0, 2) + lhs(3, 1) * rhs(0, 3),
              lhs(0, 1) * rhs(1, 0) + lhs(1, 1) * rhs(1, 1) + lhs(2, 1) * rhs(1, 2) + lhs(3, 1) * rhs(1, 3),
              lhs(0, 1) * rhs(2, 0) + lhs(1, 1) * rhs(2, 1) + lhs(2, 1) * rhs(2, 2) + lhs(3, 1) * rhs(2, 3),
              lhs(0, 1) * rhs(3, 0) + lhs(1, 1) * rhs(3, 1) + lhs(2, 1) * rhs(3, 2) + lhs(3, 1) * rhs(3, 3),
              lhs(0, 2) * rhs(0, 0) + lhs(1, 2) * rhs(0, 1) + lhs(2, 2) * rhs(0, 2) + lhs(3, 2) * rhs(0, 3),
              lhs(0, 2) * rhs(1, 0) + lhs(1, 2) * rhs(1, 1) + lhs(2, 2) * rhs(1, 2) + lhs(3, 2) * rhs(1, 3),
              lhs(0, 2) * rhs(2, 0) + lhs(1, 2) * rhs(2, 1) + lhs(2, 2) * rhs(2, 2) + lhs(3, 2) * rhs(2, 3),
              lhs(0, 2) * rhs(3, 0) + lhs(1, 2) * rhs(3, 1) + lhs(2, 2) * rhs(3, 2) + lhs(3, 2) * rhs(3, 3),
              lhs(0, 3) * rhs(0, 0) + lhs(1, 3) * rhs(0, 1) + lhs(2, 3) * rhs(0, 2) + lhs(3, 3) * rhs(0, 3),
              lhs(0, 3) * rhs(1, 0) + lhs(1, 3) * rhs(1, 1) + lhs(2, 3) * rhs(1, 2) + lhs(3, 3) * rhs(1, 3),
              lhs(0, 3) * rhs(2, 0) + lhs(1, 3) * rhs(2, 1) + lhs(2, 3) * rhs(2, 2) + lhs(3, 3) * rhs(2, 3),
              lhs(0, 3) * rhs(3, 0) + lhs(1, 3) * rhs(3, 1) + lhs(2, 3) * rhs(3, 2) + lhs(3, 3) * rhs(3, 3));
};

/******************************************************************************/
inline Vec3 operator*(const Mat4& lhs, const Vec3& rhs)
{
  // This operation assumes a w-coordinate of 1.0.
  return Vec3(lhs(0, 0) * rhs.x + lhs(1, 0) * rhs.y + lhs(2, 0) * rhs.z + lhs(3, 0) * 1.0,
              lhs(0, 1) * rhs.x + lhs(1, 1) * rhs.y + lhs(2, 1) * rhs.z + lhs(3, 1) * 1.0,
              lhs(0, 2) * rhs.x + lhs(1, 2) * rhs.y + lhs(2, 2) * rhs.z + lhs(3, 2) * 1.0);
};

} // namespace Kuma3D

#endif
