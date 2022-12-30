#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <Vec3.hpp>

namespace Kuma3D {

struct Light
{
  Vec3 mColor { 1, 1, 1 };
  float mIntensity { 1 };
};

} // namespace Kuma3D

#endif
