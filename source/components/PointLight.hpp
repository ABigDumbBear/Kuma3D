#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include <Vec3.hpp>

namespace Kuma3D {

struct PointLight
{
  float mConstant { 1 };
  float mLinear { 1 };
  float mQuadratic { 1 };

  Vec3 mAmbientColor { 1, 1, 1 };
  Vec3 mDiffuseColor { 1, 1, 1 };
  Vec3 mSpecularColor { 1, 1, 1 };
};

} // namespace Kuma3D

#endif
