#ifndef ORBIT_HPP
#define ORBIT_HPP

#include <Vec3.hpp>

namespace Lighting {

struct Orbit
{
  Kuma3D::Vec3 mAxis;
  float mRadius { 1 };
  float mSpeed { 15 };
};

} // namespace Lighting

#endif
