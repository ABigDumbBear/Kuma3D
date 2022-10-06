#ifndef MATHTESTS_HPP
#define MATHTESTS_HPP

#include <cassert>

#include "Mat4.hpp"
#include "Vec3.hpp"

#include "MathUtil.hpp"

namespace Kuma3D {

inline void TestVec3Addition()
{

  Vec3 a(5.0, 10.0, 15.0);
  Vec3 b(20.0, 35.0, 50.0);
  Vec3 c = a + b;

  assert(c.x == 25.0);
  assert(c.y == 45.0);
  assert(c.z == 65.0);
}

inline void TestVec3Subtraction()
{
  Vec3 a(5.0, 10.0, 15.0);
  Vec3 b(20.0, 35.0, 50.0);
  Vec3 c = b - a;

  assert(c.x == 15.0);
  assert(c.y == 25.0);
  assert(c.z == 35.0);
}

inline void TestMat4Multiplication()
{
  Mat4 a(1.0, 2.0, 3.0, 4.0,
         5.0, 6.0, 7.0, 8.0,
         9.0, 10.0, 11.0, 12.0,
         13.0, 14.0, 15.0, 16.0);
  Mat4 b(17.0, 18.0, 19.0, 20.0,
         21.0, 22.0, 23.0, 24.0,
         25.0, 26.0, 27.0, 28.0,
         29.0, 30.0, 31.0, 32.0);
  Mat4 c = a * b;

  assert(c(0, 0) == 250.0);
  assert(c(1, 0) == 260.0);
  assert(c(2, 0) == 270.0);
  assert(c(3, 0) == 280.0);
  assert(c(0, 1) == 618.0);
  assert(c(1, 1) == 644.0);
  assert(c(2, 1) == 670.0);
  assert(c(3, 1) == 696.0);
  assert(c(0, 2) == 986.0);
  assert(c(1, 2) == 1028.0);
  assert(c(2, 2) == 1070.0);
  assert(c(3, 2) == 1112.0);
  assert(c(0, 3) == 1354.0);
  assert(c(1, 3) == 1412.0);
  assert(c(2, 3) == 1470.0);
  assert(c(3, 3) == 1528.0);
}

inline void TestMat4MultiplicationWithVec3()
{
  Mat4 a(1.0, 2.0, 3.0, 4.0,
         5.0, 6.0, 7.0, 8.0,
         9.0, 10.0, 11.0, 12.0,
         13.0, 14.0, 15.0, 16.0);
  Vec3 b(17.0, 22.0, 35.0);
  Vec3 c = a * b;

  assert(c.x == 170.0);
  assert(c.y == 470.0);
  assert(c.z == 770.0);
}

inline void TestMat4Translation()
{
  Vec3 pos(1.0, 2.0, 3.0);
  Vec3 dest(3.0, 4.0, 5.0);

  auto translationMatrix = Translate(dest);
  pos = translationMatrix * pos;

  assert(pos.x == 4.0);
  assert(pos.y == 6.0);
  assert(pos.z == 8.0);
}

inline void TestMat4Rotation()
{
  Vec3 pos(1.0, 2.0, 3.0);
  float degrees = 90.0;

  auto rotationMatrix = Rotate(Vec3(1.0, 0.0, 0.0), degrees);
  pos = rotationMatrix * pos;

  assert(pos.x == 1.0);
  assert(pos.y == -3.0);
  assert(pos.z == 2.0);

  rotationMatrix = Rotate(Vec3(0.0, 1.0, 0.0), degrees);
  pos = rotationMatrix * pos;

  assert(pos.x == 2.0);
  assert(pos.y == -3.0);
  assert(pos.z == -1.0);

  rotationMatrix = Rotate(Vec3(0.0, 0.0, 1.0), degrees);
  pos = rotationMatrix * pos;

  assert(pos.x == 3.0);
  assert(pos.y == 2.0);
  assert(pos.z == -1.0);

  rotationMatrix = Rotate(Vec3(1.0, 0.0, 0.0), 0.0);
  rotationMatrix = Rotate(Vec3(0.0, 1.0, 0.0), 0.0) * rotationMatrix;
  rotationMatrix = Rotate(Vec3(0.0, 0.0, 1.0), 0.0) * rotationMatrix;

  assert(pos.x == 3.0);
  assert(pos.y == 2.0);
  assert(pos.z == -1.0);
}

inline void TestMat4Scaling()
{
  Vec3 pos(1.0, 2.0, 3.0);

  auto scalarMatrix = Scale(Vec3(4.0, 5.0, 6.0));
  pos = scalarMatrix * pos;

  assert(pos.x == 4.0);
  assert(pos.y == 10.0);
  assert(pos.z == 18.0);
}

inline void TestLerp()
{
  assert(Lerp(5.0, 15.0, 0.5) == 10.0);
}

} // namespace Kuma3D

#endif
