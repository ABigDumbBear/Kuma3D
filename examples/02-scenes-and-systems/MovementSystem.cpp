#include "MovementSystem.hpp"

#include <Scene.hpp>
#include <Transform.hpp>

#include <Vec3.hpp>

#include "Movable.hpp"

/******************************************************************************/
example_2::MovementSystem::MovementSystem()
  : System()
  , mPrevTime(0.0)
{
}

/******************************************************************************/
void example_2::MovementSystem::Initialize(Kuma3D::Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Movable>())
  {
    aScene.RegisterComponentType<Movable>();
  }

  if(!aScene.IsComponentTypeRegistered<Kuma3D::Transform>())
  {
    aScene.RegisterComponentType<Kuma3D::Transform>();
  }

  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Movable>()] = true;
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void example_2::MovementSystem::Operate(Kuma3D::Scene& aScene,
                                        double aTime)
{
  for(const auto& entity : GetEntities())
  {
    auto& movable = aScene.GetComponentForEntity<Movable>(entity);
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);

    auto dt = aTime - mPrevTime;
    auto distance = movable.mSpeed * dt;
    Kuma3D::Vec3 distanceVector = Kuma3D::Vec3(distance * movable.mDirection.x,
                                                           distance * movable.mDirection.y,
                                                           distance * movable.mDirection.z);
    transform.mPosition += distanceVector;
  }

  mPrevTime = aTime;
}
