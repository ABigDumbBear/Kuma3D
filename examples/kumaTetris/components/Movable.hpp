#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include <Vec3.hpp>

namespace KumaTetris {

struct Movable
{
  Kuma3D::Vec3 mTargetPosition;
  Kuma3D::Vec3 mTargetRotation;

  double mMoveSpeed { 0.0 };
  double mRotateSpeed { 0.0 };

  bool mFinishedMoving { false };
  bool mFinishedRotating { false };
};

} // namespace KumaTetris

#endif
