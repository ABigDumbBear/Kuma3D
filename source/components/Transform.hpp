#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "Scene.hpp"
#include "Types.hpp"

#include "MathUtil.hpp"
#include "Mat4.hpp"
#include "Vec3.hpp"

namespace Kuma3D {

class Transform
{
  public:
    void SetPosition(const Vec3& aPosition)
    {
      mLocalPosition = aPosition;
      CalculateTranslationMatrix();
      CalculateLocalToWorld();
    }

    void SetRotation(const Vec3& aRotation)
    {
      mLocalRotation = aRotation;
      CalculateRotationMatrix();
      CalculateLocalToWorld();
    }

    void SetScalar(const Vec3& aScalar)
    {
      mLocalScalar = aScalar;
      CalculateScalarMatrix();
      CalculateLocalToWorld();
    }

    Vec3 GetWorldPosition(Scene& aScene) const
    {
      Vec3 position = mLocalPosition;
      if(mUseParent)
      {
        auto& parentTransform = aScene.GetComponentForEntity<Transform>(mParent);
        position += parentTransform.GetWorldPosition(aScene);
      }
      return position;
    }

    Vec3 GetWorldRotation(Scene& aScene) const
    {
      Vec3 rotation = mLocalRotation;
      if(mUseParent)
      {
        auto& parentTransform = aScene.GetComponentForEntity<Transform>(mParent);
        rotation += parentTransform.GetWorldRotation(aScene);
      }
      return rotation;
    }

    Vec3 GetWorldScalar(Scene& aScene) const
    {
      Vec3 scalar = mLocalScalar;
      if(mUseParent)
      {
        auto& parentTransform = aScene.GetComponentForEntity<Transform>(mParent);
        scalar *= parentTransform.GetWorldScalar(aScene);
      }
      return scalar;
    }

    const Mat4& GetTranslationMatrix() const { return mTranslationMatrix; }
    const Mat4& GetRotationMatrix() const { return mRotationMatrix; }
    const Mat4& GetScalarMatrix() const { return mScalarMatrix; }

    const Mat4& GetLocalToWorld() const { return mLocalToWorld; }

    Entity mParent { 0 };
    bool mUseParent { false };

  private:
    void CalculateTranslationMatrix()
    {
      mTranslationMatrix = Translate(mLocalPosition);
    }

    void CalculateRotationMatrix()
    {
      mRotationMatrix = Rotate(Vec3(1.0, 0.0, 0.0), mLocalRotation.x);
      mRotationMatrix = mRotationMatrix * Rotate(Vec3(0.0, 1.0, 0.0), mLocalRotation.y);
      mRotationMatrix = mRotationMatrix * Rotate(Vec3(0.0, 0.0, 1.0), mLocalRotation.z);
    }

    void CalculateScalarMatrix()
    {
      mScalarMatrix = Scale(mLocalScalar);
    }

    void CalculateLocalToWorld()
    {
      mLocalToWorld = mTranslationMatrix * mRotationMatrix * mScalarMatrix;
    }

    Vec3 mLocalPosition;
    Vec3 mLocalRotation;
    Vec3 mLocalScalar { 1.0, 1.0, 1.0 };

    Mat4 mTranslationMatrix;
    Mat4 mRotationMatrix;
    Mat4 mScalarMatrix;

    Mat4 mLocalToWorld;
};

} // namespace Kuma3D

#endif
