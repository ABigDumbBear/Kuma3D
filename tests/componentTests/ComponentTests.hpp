#ifndef COMPONENTTESTS_HPP
#define COMPONENTTESTS_HPP

#include <cassert>

#include <Scene.hpp>
#include <Transform.hpp>

namespace Kuma3D {

inline void TestTransformPosition()
{
  Scene scene;
  scene.RegisterComponentType<Transform>();

  auto child = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(child);
  auto parent = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(parent);
  auto grandparent = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(grandparent);

  auto& childTransform = scene.GetComponentForEntity<Transform>(child);
  auto& parentTransform = scene.GetComponentForEntity<Transform>(parent);
  auto& grandparentTransform = scene.GetComponentForEntity<Transform>(grandparent);

  childTransform.SetPosition(Vec3(1.0, 2.0, 3.0));
  childTransform.mParent = parent;
  childTransform.mUseParent = true;
  parentTransform.SetPosition(Vec3(5.0, -6.0, 7.0));
  parentTransform.mParent = grandparent;
  parentTransform.mUseParent = true;
  grandparentTransform.SetPosition(Vec3(4.0, 2.0, 9.0));

  auto childPosition = childTransform.GetWorldPosition(scene);
  auto parentPosition = parentTransform.GetWorldPosition(scene);
  auto grandparentPosition = grandparentTransform.GetWorldPosition(scene);

  assert(grandparentPosition.x == 4.0);
  assert(grandparentPosition.y == 2.0);
  assert(grandparentPosition.z == 9.0);

  assert(parentPosition.x == 9.0);
  assert(parentPosition.y == -4.0);
  assert(parentPosition.z == 16.0);

  assert(childPosition.x == 10.0);
  assert(childPosition.y == -2.0);
  assert(childPosition.z == 19.0);
}

inline void TestTransformRotation()
{
  Scene scene;
  scene.RegisterComponentType<Transform>();

  auto child = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(child);
  auto parent = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(parent);
  auto grandparent = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(grandparent);

  auto& childTransform = scene.GetComponentForEntity<Transform>(child);
  auto& parentTransform = scene.GetComponentForEntity<Transform>(parent);
  auto& grandparentTransform = scene.GetComponentForEntity<Transform>(grandparent);

  childTransform.SetRotation(Vec3(1.0, 2.0, 3.0));
  childTransform.mParent = parent;
  childTransform.mUseParent = true;
  parentTransform.SetRotation(Vec3(5.0, -6.0, 7.0));
  parentTransform.mParent = grandparent;
  parentTransform.mUseParent = true;
  grandparentTransform.SetRotation(Vec3(4.0, 2.0, 9.0));

  auto childRotation = childTransform.GetWorldRotation(scene);
  auto parentRotation = parentTransform.GetWorldRotation(scene);
  auto grandparentRotation = grandparentTransform.GetWorldRotation(scene);

  assert(grandparentRotation.x == 4.0);
  assert(grandparentRotation.y == 2.0);
  assert(grandparentRotation.z == 9.0);

  assert(parentRotation.x == 9.0);
  assert(parentRotation.y == -4.0);
  assert(parentRotation.z == 16.0);

  assert(childRotation.x == 10.0);
  assert(childRotation.y == -2.0);
  assert(childRotation.z == 19.0);
}

inline void TestTransformScaling()
{
  Scene scene;
  scene.RegisterComponentType<Transform>();

  auto child = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(child);
  auto parent = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(parent);
  auto grandparent = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(grandparent);

  auto& childTransform = scene.GetComponentForEntity<Transform>(child);
  auto& parentTransform = scene.GetComponentForEntity<Transform>(parent);
  auto& grandparentTransform = scene.GetComponentForEntity<Transform>(grandparent);

  childTransform.SetScalar(Vec3(1.0, 2.0, 3.0));
  childTransform.mParent = parent;
  childTransform.mUseParent = true;
  parentTransform.SetScalar(Vec3(5.0, 6.0, 7.0));
  parentTransform.mParent = grandparent;
  parentTransform.mUseParent = true;
  grandparentTransform.SetScalar(Vec3(4.0, 2.0, 9.0));

  auto childScalar = childTransform.GetWorldScalar(scene);
  auto parentScalar = parentTransform.GetWorldScalar(scene);
  auto grandparentScalar = grandparentTransform.GetWorldScalar(scene);

  assert(grandparentScalar.x == 4.0);
  assert(grandparentScalar.y == 2.0);
  assert(grandparentScalar.z == 9.0);

  assert(parentScalar.x == 20.0);
  assert(parentScalar.y == 12.0);
  assert(parentScalar.z == 63.0);

  assert(childScalar.x == 20.0);
  assert(childScalar.y == 24.0);
  assert(childScalar.z == 189.0);
}

} // namespace Kuma3D

#endif
