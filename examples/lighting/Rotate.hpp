#ifndef ROTATE_HPP
#define ROTATE_HPP

#include "Vec3.hpp"

namespace Lighting {

struct Rotate
{
  Kuma3D::Vec3 mAngles;
  double mSpeed { 100 };
};

} // namespace Lighting

#endif
