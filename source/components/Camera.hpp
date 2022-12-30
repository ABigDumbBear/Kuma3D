#ifndef CAMERA_HPP
#define CAMERA_HPP

namespace Kuma3D {

struct Camera
{
  float mViewportX { 1280.0 };
  float mViewportY { 720.0 };
  bool mUseWindowAsViewport { true };

  float mFOV { 45.0 };
  float mNearPlane { 0.1 };
  float mFarPlane { 1000.0 };
};

} // namespace Kuma3D

#endif
