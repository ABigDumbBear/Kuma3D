#include "PhysicsSystem.hpp"

#include <Scene.hpp>
#include <Transform.hpp>

#include "Physics.hpp"

namespace Cubes {

/******************************************************************************/
void PhysicsSystem::Initialize(Kuma3D::Scene& aScene)
{
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<Physics>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void PhysicsSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  auto dt = aTime - mTime;
  for(const auto& entity : GetEntities())
  {
    auto& physics = aScene.GetComponentForEntity<Physics>(entity);
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);

    physics.mVelocity += (physics.mAcceleration * dt);
    transform.mPosition += (physics.mVelocity * dt) + (physics.mAcceleration * dt * dt * 0.5);
  }

  mTime = aTime;
}

} // namespace Cubes
