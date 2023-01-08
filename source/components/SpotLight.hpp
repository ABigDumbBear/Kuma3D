#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

#include <Vec3.hpp>

namespace Kuma3D {

struct SpotLight
{
  Vec3 mDirection;

  float mCutoffAngle;

  Vec3 mAmbientColor { 1, 1, 1 };
  Vec3 mDiffuseColor { 1, 1, 1 };
  Vec3 mSpecularColor { 1, 1, 1 };
};

} // namespace Kuma3D

#endif
