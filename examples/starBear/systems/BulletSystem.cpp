#include "BulletSystem.hpp"

#include <Scene.hpp>

#include <Transform.hpp>

#include "Bullet.hpp"

namespace StarBear {

/******************************************************************************/
void BulletSystem::Initialize(Kuma3D::Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Kuma3D::Transform>())
  {
    aScene.RegisterComponentType<Kuma3D::Transform>();
  }

  if(!aScene.IsComponentTypeRegistered<Bullet>())
  {
    aScene.RegisterComponentType<Bullet>();
  }

  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<Bullet>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void BulletSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  auto dt = aTime - mTimeSinceLastUpdate;

  for(const auto& entity : GetEntities())
  {
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);
    auto& bullet = aScene.GetComponentForEntity<Bullet>(entity);

    auto bulletPosition = transform.GetWorldPosition(aScene);
    Kuma3D::Vec3 direction(0.0, 0.0, -1.0);
    auto d = direction * (bullet.mSpeed * dt);
    bulletPosition += direction;

    if(bulletPosition.z <= -250)
    {
      aScene.RemoveEntity(entity);
    }
    //transform.SetPosition(bulletPosition);
  }

  mTimeSinceLastUpdate = aTime;
}

} // namespace StarBear
