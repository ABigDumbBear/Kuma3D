#include "ShipControlSystem.hpp"

#include <cmath>

#include <Scene.hpp>
#include <MathUtil.hpp>

#include <Transform.hpp>

#include "Bullet.hpp"
#include "BulletUtil.hpp"
#include "ShipControl.hpp"

namespace StarBear {

/******************************************************************************/
void ShipControlSystem::Initialize(Kuma3D::Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Kuma3D::Transform>())
  {
    aScene.RegisterComponentType<Kuma3D::Transform>();
  }

  if(!aScene.IsComponentTypeRegistered<Bullet>())
  {
    aScene.RegisterComponentType<Bullet>();
  }

  if(!aScene.IsComponentTypeRegistered<ShipControl>())
  {
    aScene.RegisterComponentType<ShipControl>();
  }

  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Kuma3D::Transform>()] = true;
  signature[aScene.GetComponentIndex<ShipControl>()] = true;
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
void ShipControlSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  auto dt = aTime - mTimeSinceLastUpdate;
  for(const auto& entity : GetEntities())
  {
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);
    auto& control = aScene.GetComponentForEntity<ShipControl>(entity);

    // Calculate speed and distance.
    auto si = control.mCurrentSpeed;
    auto a = control.mAcceleration;
    auto sf = std::min(control.mMaxSpeed, (a * dt) + si);
    auto d = sf * dt;

    // Move the ship transform according to which keys are pressed (if any).
    bool movingShip = false;
    for(const auto& key : mPressedKeys)
    {
      switch(key)
      {
        case Kuma3D::KeyCode::eKEY_LEFT:
        {
          movingShip = true;

          Kuma3D::Vec3 direction(-1.0, 0.0, 0.0);
          direction *= d;
          transform.mPosition += direction;
          transform.mPosition.x = std::max(-30.0f, transform.mPosition.x);
          transform.mRotation.z = std::min(control.mMaxRotation, transform.mRotation.z + d);

          control.mCurrentSpeed = sf;

          break;
        }
        case Kuma3D::KeyCode::eKEY_RIGHT:
        {
          movingShip = true;

          Kuma3D::Vec3 direction(1.0, 0.0, 0.0);
          direction *= d;
          transform.mPosition.x += d;
          transform.mPosition.x = std::min(30.0f, transform.mPosition.x);
          transform.mRotation.z = std::max(-control.mMaxRotation, transform.mRotation.z - d);

          control.mCurrentSpeed = sf;

          break;
        }
        case Kuma3D::KeyCode::eKEY_UP:
        {
          movingShip = true;

          Kuma3D::Vec3 direction(0.0, 1.0, 0.0);
          direction *= d;
          transform.mPosition += direction;
          transform.mPosition.y = std::min(15.0f, transform.mPosition.y);

          control.mCurrentSpeed = sf;

          break;
        }
        case Kuma3D::KeyCode::eKEY_DOWN:
        {
          movingShip = true;

          Kuma3D::Vec3 direction(0.0, -1.0, 0.0);
          direction *= d;
          transform.mPosition += direction;
          transform.mPosition.y = std::max(-15.0f, transform.mPosition.y);

          control.mCurrentSpeed = sf;

          break;
        }
        case Kuma3D::KeyCode::eKEY_SPACE:
        {
          auto bulletDelta = aTime - mTimeSinceLastBullet;
          auto secondsBetweenBullets = 1.0 / control.mRateOfFire;

          if(bulletDelta >= secondsBetweenBullets)
          {
            SpawnBullet(aScene, transform.mPosition);
            mTimeSinceLastBullet = aTime;
          }

          break;
        }
        default:
        {
          break;
        }
      }
    }

    // If the ship is not moving, revert to the neutral position.
    if(!movingShip)
    {
      control.mCurrentSpeed = 0.0;
      transform.mRotation = Kuma3D::Lerp(transform.mRotation, Kuma3D::Vec3(0, 0, 0), 0.1);
    }
  }

  // Reset the update timer.
  mTimeSinceLastUpdate = aTime;
}

/******************************************************************************/
void ShipControlSystem::HandleKeyPressed(const Kuma3D::KeyCode& aKey, int aMods)
{
  mPressedKeys.emplace_back(aKey);
}

/******************************************************************************/
void ShipControlSystem::HandleKeyReleased(const Kuma3D::KeyCode& aKey, int aMods)
{
  auto foundKey = std::find(mPressedKeys.begin(), mPressedKeys.end(), aKey);
  if(foundKey != mPressedKeys.end())
  {
    mPressedKeys.erase(foundKey);
  }
}

/******************************************************************************/
void ShipControlSystem::SpawnBullet(Kuma3D::Scene& aScene, const Kuma3D::Vec3& aPosition)
{
  auto bullet = aScene.CreateEntity();
  Kuma3D::Transform bulletTransform;
  bulletTransform.mPosition = aPosition;
  aScene.AddComponentToEntity<Kuma3D::Transform>(bullet, bulletTransform);

  auto bulletMesh = CreateCube(1, Kuma3D::Vec3(1.0, 1.0, 1.0));
  aScene.AddComponentToEntity<Kuma3D::Mesh>(bullet, bulletMesh);

  aScene.AddComponentToEntity<Bullet>(bullet);
}

} // namespace StarBear
