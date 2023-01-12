#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Entity.hpp"

#include "Mat4.hpp"
#include "Vec3.hpp"

namespace Kuma3D {

struct Transform
{
  Vec3 mPosition;
  Mat4 mRotation;
  Vec3 mScalar { 1.0, 1.0, 1.0 };

  Entity mParent;
  bool mUseParent { false };
};

} // namespace Kuma3D

#endif
