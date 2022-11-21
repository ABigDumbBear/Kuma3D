#ifndef CORETESTS_HPP
#define CORETESTS_HPP

#include <cassert>

#include <ComponentList.hpp>
#include <Scene.hpp>

#include <Signature.hpp>

#include <Transform.hpp>

namespace Kuma3D {

/******************************************************************************/
inline void TestComponentListAddition()
{
  ComponentListT<Transform> list;
  Transform transform;
  transform.mPosition.x = 55.0;

  list.AddComponentToEntity(0, transform);

  bool accessFailed = false;
  try
  {
    transform = list.GetComponentForEntity(0);
    assert(transform.mPosition.x == 55.0);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }

  assert(!accessFailed);
}

/******************************************************************************/
inline void TestComponentListRemoval()
{
  ComponentListT<Transform> list;

  Transform transform;
  list.AddComponentToEntity(0, transform);
  list.RemoveComponentFromEntity(0);

  bool accessFailed = false;
  try
  {
    transform = list.GetComponentForEntity(0);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }

  assert(accessFailed);
}

/******************************************************************************/
inline void TestComponentListMerge()
{
  ComponentListT<Transform> listA;
  ComponentListT<Transform> listB;

  Transform transformA;
  transformA.mPosition.x = 55;
  listA.AddComponentToEntity(0, transformA);
  listB.Merge(listA);

  // Check that the Transform was moved to List B.
  auto& transformB = listB.GetComponentForEntity(0);
  assert(transformB.mPosition.x == 55);

  // Check that there is no longer any Transform in List A.
  bool accessFailed = false;
  try
  {
    auto& transformC = listA.GetComponentForEntity(0);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }
  assert(accessFailed);
}

/******************************************************************************/
inline void TestEntityRemoval()
{
  // Create a Scene and register the Transform component.
  Scene scene;
  scene.RegisterComponentType<Transform>();

  // Create an Entity, add a Transform to it, and update the Scene.
  auto entity = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(entity);
  scene.OperateSystems(0);

  // Remove the Entity and update the Scene.
  scene.RemoveEntity(entity);
  scene.OperateSystems(0);

  assert(!scene.DoesEntityHaveComponent<Transform>(0));
}

/******************************************************************************/
inline void TestSignatureRelevancyCheck()
{
  Signature signatureA { false, false, true, false, true, false };
  Signature signatureB { true, true, true, false, true, false };

  assert(IsSignatureRelevant(signatureA, signatureB));

  signatureB[2] = false;

  assert(!IsSignatureRelevant(signatureA, signatureB));
}

} // namespace Kuma3D

#endif
