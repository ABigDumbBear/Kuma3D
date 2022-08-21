#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Vec3.hpp"

namespace Kuma3D {

struct Transform
{
  Vec3 mPosition;
  Vec3 mRotation;
  Vec3 mScalar { 1.0, 1.0, 1.0 };
};

} // namespace Kuma3D

#endif
