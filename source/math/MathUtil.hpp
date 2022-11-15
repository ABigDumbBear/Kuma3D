#ifndef MATHUTIL_HPP
#define MATHUTIL_HPP

#include <math.h>

#include "Camera.hpp"

#include "Mat4.hpp"
#include "Vec3.hpp"

namespace Kuma3D {

/**
 * Creates and returns the normalized version of a given vector.
 *
 * @param aVector The vector to normalize.
 * @return The normalized version of the given vector.
 */
inline Vec3 Normalize(const Vec3& aVector)
{
  auto magnitude = std::sqrt(std::pow(aVector.x, 2) +
                             std::pow(aVector.y, 2) +
                             std::pow(aVector.z, 2));

  Vec3 result(0, 0, 0);
  if(magnitude > 0)
  {
    result.x = aVector.x / magnitude;
    result.y = aVector.y / magnitude;
    result.z = aVector.z / magnitude;
  }

  return result;
}

/**
 * Calculates the cross product of two vectors and returns it.
 *
 * @param aVectorA The first vector in the equation.
 * @param aVectorB The second vector in the equation.
 * @return The cross product of the two vectors.
 */
inline Vec3 Cross(const Vec3& aVectorA, const Vec3& aVectorB)
{
  auto yz = aVectorA.y * aVectorB.z;
  auto zy = aVectorA.z * aVectorB.y;
  auto zx = aVectorA.z * aVectorB.x;
  auto xz = aVectorA.x * aVectorB.z;
  auto xy = aVectorA.x * aVectorB.y;
  auto yx = aVectorA.y * aVectorB.x;

  return Vec3(yz - zy, zx - xz, xy - yx);
}

/**
 * Calculates the dot product of two vectors and returns it.
 *
 * @param aVectorA The first vector in the equation.
 * @param aVectorB The second vector in the equation.
 * @return The dot product of the two vectors.
 */
inline float Dot(const Vec3& aVectorA, const Vec3& aVectorB)
{
  auto xx = aVectorA.x * aVectorB.x;
  auto yy = aVectorA.y * aVectorB.y;
  auto zz = aVectorA.z * aVectorB.z;

  return (xx + yy + zz);
}

/**
 * Calculates the distance between two vectors and returns it.
 *
 * @param aVectorA The first vector.
 * @param aVectorB The second vector.
 * @return The distance between the two vectors.
 */
inline float Distance(const Vec3& aVectorA, const Vec3& aVectorB)
{
  auto xVal = aVectorB.x - aVectorA.x;
  auto yVal = aVectorB.y - aVectorA.y;
  auto zVal = aVectorB.z - aVectorA.z;

  return std::sqrt(std::pow(xVal, 2) + std::pow(yVal, 2) + std::pow(zVal, 2));
}

/**
 * Creates and returns a scalar transformation matrix for the
 * given scalar vector.
 *
 * @param aVector The scalar vector.
 * @return A scalar transformation matrix.
 */
inline Mat4 Scale(const Vec3& aVector)
{
  return Mat4(aVector.x, 0.0, 0.0, 0.0,
              0.0, aVector.y, 0.0, 0.0,
              0.0, 0.0, aVector.z, 0.0,
              0.0, 0.0, 0.0, 1.0);
}

/**
 * Creates and returns a translation transformation matrix for the
 * given destination vector.
 *
 * @param aVector The destination vector.
 * @return A translation transformation matrix.
 */
inline Mat4 Translate(const Vec3& aVector)
{
  return Mat4(1.0, 0.0, 0.0, aVector.x,
              0.0, 1.0, 0.0, aVector.y,
              0.0, 0.0, 1.0, aVector.z,
              0.0, 0.0, 0.0, 1.0);
}

/**
 * Creates and returns a rotation transformation matrix for a given
 * number of degrees around a given axis. Note that the given axis
 * must be normalized before calling this function!
 *
 * @param aVector The normalized axis of rotation.
 * @param aDegrees The amount of degrees to rotate.
 * @return A rotation transformation matrix.
 */
inline Mat4 Rotate(const Vec3& aVector,
                   float aDegrees)
{
  float c = std::cos(aDegrees * (M_PI / 180.0));
  float s = std::sin(aDegrees * (M_PI / 180.0));
  float d = 1.0 - c;

  float vx = aVector.x;
  float vy = aVector.y;
  float vz = aVector.z;

  float vx2 = std::pow(vx, 2) * d;
  float vy2 = std::pow(vy, 2) * d;
  float vz2 = std::pow(vz, 2) * d;

  float vxvy = (vx * vy) * d;
  float vxvz = (vx * vz) * d;
  float vyvz = (vy * vz) * d;

  return Mat4(c + vx2, vxvy - s * vz, vxvz + s * vy, 0.0,
              vxvy + s * vz, c + vy2, vyvz - s * vx, 0.0,
              vxvz - s * vy, vyvz + s * vx, c + vz2, 0.0,
              0.0, 0.0, 0.0, 1.0);
}

/**
 * Creates and returns a view matrix for a given direction vector,
 * right vector, and camera position.
 *
 * @param aDirectionVector A vector pointing in the direction the camera
 *                         is looking.
 * @param aRightVector A vector pointing to the right of the camera.
 * @param aPosition The position of the camera.
 */
inline Mat4 View(const Vec3& aDirectionVector,
                 const Vec3& aRightVector,
                 const Vec3& aPosition)
{
  auto upVector = Cross(aDirectionVector, aRightVector);
  Mat4 axesMatrix = Mat4(aRightVector.x, aRightVector.y, aRightVector.z, 0.0,
                         upVector.x, upVector.y, upVector.z, 0.0,
                         aDirectionVector.x, aDirectionVector.y, aDirectionVector.z, 0.0,
                         0.0, 0.0, 0.0, 1.0);
  Mat4 translationMatrix = Mat4(1.0, 0.0, 0.0, -aPosition.x,
                                0.0, 1.0, 0.0, -aPosition.y,
                                0.0, 0.0, 1.0, -aPosition.z,
                                0.0, 0.0, 0.0, 1.0);

  return axesMatrix * translationMatrix;
}

/**
 * Creates and returns a perspective projection matrix for the given camera.
 *
 * @param aCamera The camera to create a matrix for.
 */
inline Mat4 Perspective(const Camera& aCamera)
{
  auto rad = aCamera.mFOV * (M_PI / 180.0);
  auto f = 1.0 / std::tan(rad * 0.5);
  auto aspect = aCamera.mViewportX / aCamera.mViewportY;
  auto far = aCamera.mFarPlane;
  auto near = aCamera.mNearPlane;

  return Mat4(f / aspect, 0.0, 0.0, 0.0,
              0.0, f, 0.0, 0.0,
              0.0, 0.0, (near + far) / (near - far), (2 * far * near) / (near - far),
              0.0, 0.0, -1.0, 0.0);
}

/**
 * Creates and returns an orthographic projection matrix for the given camera.
 * It's assumed that the left and bottom values are 0.0.
 *
 * @param aCamera The camera to create a matrix for.
 */
inline Mat4 Orthographic(const Camera& aCamera)
{
  auto right = aCamera.mViewportX;
  auto top = aCamera.mViewportY;
  auto far = aCamera.mFarPlane;
  auto near = aCamera.mNearPlane;

  return Mat4(2.0 / right, 0.0, 0.0, -1.0,
              0.0, 2.0 / top, 0.0, -1.0,
              0.0, 0.0, 1.0 / (far - near), -near / (far - near),
              0.0, 0.0, 0.0, 1.0);
}

/**
 * Calculates and returns the linear interpolation between two values
 * and a third percentage value.
 *
 * @param aStart The starting value.
 * @param aTarget The target value.
 * @param aPercent The percentage to interpolate by, between 0.0 and 1.0.
 * @return The value that is aPercent of the way towards aTarget from aStart.
 */
inline float Lerp(float aStart, float aTarget, float aPercent)
{
  return (aStart + (aTarget - aStart) * aPercent);
}

/**
 * Performs linear interpolation on each component of a Vec3.
 *
 * @param aStart The staring Vec3.
 * @param aTarget The target Vec3.
 * @param aPercent The percentage to interpolate by, between 0.0 and 1.0.
 * @return The Vec3 that is aPercent of the way towards aTarget from aStart.
 */
inline Vec3 Lerp(const Vec3& aStart, const Vec3& aTarget, float aPercent)
{
  return Vec3(Lerp(aStart.x, aTarget.x, aPercent),
              Lerp(aStart.y, aTarget.y, aPercent),
              Lerp(aStart.z, aTarget.z, aPercent));
}

} // namespace Kuma3D

#endif
