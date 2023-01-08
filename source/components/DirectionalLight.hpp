#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include <Vec3.hpp>

namespace Kuma3D {

struct DirectionalLight
{
  Vec3 mDirection;

  Vec3 mAmbientColor { 1, 1, 1 };
  Vec3 mDiffuseColor { 1, 1, 1 };
  Vec3 mSpecularColor { 1, 1, 1 };
};

} // namespace Kuma3D

#endif
