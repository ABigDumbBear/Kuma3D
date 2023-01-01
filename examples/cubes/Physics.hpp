#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <Vec3.hpp>

namespace Cubes {

struct Physics
{
  Kuma3D::Vec3 mAcceleration;
  Kuma3D::Vec3 mVelocity;
};

} // namespace Cubes

#endif
