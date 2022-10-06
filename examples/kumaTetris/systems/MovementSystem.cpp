#include "MovementSystem.hpp"

#include <Scene.hpp>
#include <Transform.hpp>

#include <Vec3.hpp>
#include <MathUtil.hpp>

#include "Movable.hpp"

namespace KumaTetris {

const float MovementSystem::mEpsilon = 0.0005;

/******************************************************************************/
void MovementSystem::Initialize(Kuma3D::Scene& aScene)
{
  // Register the relevant components.
  if(!aScene.IsComponentTypeRegistered<Kuma3D::Transform>())
  {
    aScene.RegisterComponentType<Kuma3D::Transform>();
  }

  if(!aScene.IsComponentTypeRegistered<Movable>())
  {
    aScene.RegisterComponentType<Movable>();
  }

  // Set the signature.
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<Movable>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void MovementSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  for(const auto& entity : GetEntities())
  {
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);
    auto& movable = aScene.GetComponentForEntity<Movable>(entity);

    // Handle moving the Entity.
    if(!movable.mFinishedMoving)
    {
      // Calculate the new position using linear interpolation.
      auto newPosition = Kuma3D::Lerp(transform.mPosition,
                                      movable.mTargetPosition,
                                      movable.mMoveSpeed);

      // If the new position is close enough to the target, then set it
      // equal to the target.
      if(std::abs(newPosition.x - movable.mTargetPosition.x) <= mEpsilon &&
         std::abs(newPosition.y - movable.mTargetPosition.y) <= mEpsilon &&
         std::abs(newPosition.z - movable.mTargetPosition.z) <= mEpsilon)
      {
        transform.mPosition = movable.mTargetPosition;
        movable.mFinishedMoving = true;
      }
      else
      {
        transform.mPosition = newPosition;
      }
    }

    // Handle rotating the Entity.
    if(!movable.mFinishedRotating)
    {
      // Calculate the new rotation using linear interpolation.
      auto newRotation = Kuma3D::Lerp(transform.mRotation,
                                      movable.mTargetRotation,
                                      movable.mRotateSpeed);

      // If the new rotation is close enough to the target, then set it
      // equal to the target.
      if(std::abs(newRotation.x - movable.mTargetRotation.x) <= mEpsilon &&
         std::abs(newRotation.y - movable.mTargetRotation.y) <= mEpsilon &&
         std::abs(newRotation.z - movable.mTargetRotation.z) <= mEpsilon)
      {
        transform.mRotation = movable.mTargetRotation;
        movable.mFinishedRotating = true;
      }
      else
      {
        transform.mRotation = newRotation;
      }
    }
  }
}

} // namespace KumaTetris
