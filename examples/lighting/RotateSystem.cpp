#include "RotateSystem.hpp"

#include <Scene.hpp>

#include <Transform.hpp>

#include "Rotate.hpp"

namespace Lighting {

/******************************************************************************/
void RotateSystem::Initialize(Kuma3D::Scene& aScene)
{
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<Rotate>()] = true;
  SetSignature(signature);

  Kuma3D::KeyPressed.Connect(mObserver, [this](const Kuma3D::KeyCode& aKey,
                                               int aMods)
  {
    this->HandleKeyPressed(aKey, aMods);
  });

  Kuma3D::KeyReleased.Connect(mObserver, [this](const Kuma3D::KeyCode& aKey,
                                                int aMods)
  {
    this->HandleKeyReleased(aKey, aMods);
  });
}

/******************************************************************************/
void RotateSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  auto dt = aTime - mTime;
  for(const auto& entity : GetEntities())
  {
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);
    auto& rotate = aScene.GetComponentForEntity<Rotate>(entity);

    auto degrees = rotate.mSpeed * dt;

    for(const auto& key : mPressedKeys)
    {
      switch(key)
      {
        case Kuma3D::KeyCode::eKEY_LEFT:
        {
          transform.mRotation.y -= degrees;
          break;
        }
        case Kuma3D::KeyCode::eKEY_RIGHT:
        {
          transform.mRotation.y += degrees;
          break;
        }
        case Kuma3D::KeyCode::eKEY_UP:
        {
          transform.mRotation.x -= degrees;
          break;
        }
        case Kuma3D::KeyCode::eKEY_DOWN:
        {
          transform.mRotation.x += degrees;
          break;
        }
        default: { break; }
      }
    }
  }

  mTime = aTime;
}

/******************************************************************************/
void RotateSystem::HandleKeyPressed(const Kuma3D::KeyCode& aKey,
                                    int aMods)
{
  mPressedKeys.emplace_back(aKey);
}

/******************************************************************************/
void RotateSystem::HandleKeyReleased(const Kuma3D::KeyCode& aKey,
                                     int aMods)
{
  auto foundKey = std::find(mPressedKeys.begin(), mPressedKeys.end(), aKey);
  if(foundKey != mPressedKeys.end())
  {
    mPressedKeys.erase(foundKey);
  }
}

} // namespace Lighting
