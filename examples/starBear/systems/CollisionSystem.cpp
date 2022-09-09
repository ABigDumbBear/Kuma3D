#include "CollisionSystem.hpp"

#include <algorithm>

#include <Scene.hpp>

#include <Transform.hpp>

#include "Hitbox.hpp"

namespace StarBear {

/******************************************************************************/
void CollisionSystem::Initialize(Kuma3D::Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Kuma3D::Transform>())
  {
    aScene.RegisterComponentType<Kuma3D::Transform>();
  }

  if(!aScene.IsComponentTypeRegistered<Hitbox>())
  {
    aScene.RegisterComponentType<Hitbox>();
  }

  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<Hitbox>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void CollisionSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  // For each new Entity, calculate which cells of the grid it's occupying.
  for(const auto& entity : mNewEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);
    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
  }
  mNewEntities.clear();
}

/******************************************************************************/
void CollisionSystem::HandleEntityBecameEligible(const Kuma3D::Entity& aEntity)
{
  mNewEntities.emplace_back(aEntity);
}

/******************************************************************************/
void CollisionSystem::HandleEntityBecameIneligible(const Kuma3D::Entity& aEntity)
{
  // Remove the Entity from the new Entities list, if it's there.
  auto foundEntity = std::find(mNewEntities.begin(), mNewEntities.end(), aEntity);
  if(foundEntity != mNewEntities.end())
  {
    mNewEntities.erase(foundEntity);
  }

  // Remove the Entity from the Entity grid.
  for(int x = 0; x < 10; ++x)
  {
    for(int y = 0; y < 10; ++y)
    {
      for(int z = 0; z < 50; ++z)
      {
        auto& entityList = mEntityGrid[x][y][z];
        auto foundEntity = std::find(entityList.begin(),
                                     entityList.end(),
                                     aEntity);

        if(foundEntity != entityList.end())
        {
          entityList.erase(foundEntity);
        }
      }
    }
  }
}

} // namespace StarBear
