#ifndef COMPONENTTESTS_HPP
#define COMPONENTTESTS_HPP

#include <cassert>

#include <Scene.hpp>
#include <Transform.hpp>

#include <iostream>

namespace Kuma3D {

inline void TestTransformPosition()
{
  Kuma3D::Scene scene;
  scene.RegisterComponentType<Kuma3D::Transform>();

  auto child = scene.CreateEntity();
  scene.AddComponentToEntity<Kuma3D::Transform>(child);
  auto& childTransform = scene.GetComponentForEntity<Kuma3D::Transform>(child);
  childTransform.SetPosition(Kuma3D::Vec3(2.0, 3.0, -4.0));

  auto parent = scene.CreateEntity();
  scene.AddComponentToEntity<Kuma3D::Transform>(parent);
  auto& parentTransform = scene.GetComponentForEntity<Kuma3D::Transform>(parent);
  parentTransform.SetPosition(Kuma3D::Vec3(-6.0, 7.0, 3.0));

  childTransform.mParent = parent;
  //childTransform.mUseParent = true;

  auto childPosition = childTransform.GetWorldPosition(scene);
  std::cout << childPosition << std::endl;
  assert(childPosition.x == -4.0);
  assert(childPosition.y == 10.0);
  assert(childPosition.z == -1.0);
}

} // namespace Kuma3D

#endif
