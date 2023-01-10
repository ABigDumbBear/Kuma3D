#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <Vec3.hpp>

namespace Kuma3D {

struct PointLight
{
  float mConstant { 1 };
  float mLinear { 1 };
  float mQuadratic { 1 };

  float mAmbientIntensity { 1 };
  float mDiffuseIntensity { 1 };
  float mSpecularIntensity { 1 };
};

} // namespace Kuma3D

#endif
