#include "OrbitSystem.hpp"

#include <Scene.hpp>
#include <MathUtil.hpp>
#include <Transform.hpp>

#include "Orbit.hpp"

#include <iostream>

namespace Lighting {

/******************************************************************************/
void OrbitSystem::Initialize(Kuma3D::Scene& aScene)
{
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<Orbit>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void OrbitSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  auto dt = aTime - mTime;
  for(const auto& entity : GetEntities())
  {
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);
    auto& orbit = aScene.GetComponentForEntity<Orbit>(entity);

    auto degrees = orbit.mSpeed * dt;
    auto rotationMatrix = Kuma3D::Rotate(orbit.mAxis, degrees);
    transform.mPosition = rotationMatrix * (transform.mPosition * orbit.mRadius);
    //transform.mPosition.x = sin(aTime) * 2;
    //transform.mPosition.y = cos(aTime) * 2;
  }

  mTime = aTime;
}

} // namespace Lighting
