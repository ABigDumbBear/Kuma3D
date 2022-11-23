#ifndef CORETESTS_HPP
#define CORETESTS_HPP

#include <cassert>

#include <ComponentList.hpp>
#include <Scene.hpp>

#include <Signature.hpp>

namespace Kuma3D {

struct TestComponentA
{
  int mValue { 0 };
};

struct TestComponentB
{
  std::string mValue;
};

/******************************************************************************/
inline void TestComponentListAddition()
{
  ComponentListT<TestComponentA> list;
  TestComponentA component;
  component.mValue = 55;

  list.AddComponentToEntity(0, component);

  bool accessFailed = false;
  try
  {
    component = list.GetComponentForEntity(0);
    assert(component.mValue == 55);
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
  ComponentListT<TestComponentA> list;

  TestComponentA transform;
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
  ComponentListT<TestComponentA> listA;
  ComponentListT<TestComponentA> listB;

  TestComponentA componentA;
  componentA.mValue = 55;
  listA.AddComponentToEntity(0, componentA);
  listB.Merge(listA);

  // Check that the TestComponentA was moved to List B.
  auto& componentB = listB.GetComponentForEntity(0);
  assert(componentB.mValue == 55);

  // Check that there is no longer any TestComponentA in List A.
  bool accessFailed = false;
  try
  {
    auto& componentC = listA.GetComponentForEntity(0);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }
  assert(accessFailed);
}

/******************************************************************************/
inline void TestEntityAddition()
{
  // Create a Scene and register the TestComponentA component.
  Scene scene;
  scene.RegisterComponentType<TestComponentA>();

  // Create an Entity, add a TestComponentA to it, and update the Scene.
  auto entity = scene.CreateEntity();
  TestComponentA component;
  component.mValue = 55;
  scene.AddComponentToEntity<TestComponentA>(entity, component);
  scene.OperateSystems(0);

  assert(scene.DoesEntityHaveComponent<TestComponentA>(entity));

  // Retrieve the TestComponentA and check it.
  auto& componentCheck = scene.GetComponentForEntity<TestComponentA>(entity);
  assert(componentCheck.mValue == 55);
}

/******************************************************************************/
inline void TestEntityRemoval()
{
  // Create a Scene and register the TestComponentA component.
  Scene scene;
  scene.RegisterComponentType<TestComponentA>();

  // Create an Entity, add a TestComponentA to it, and update the Scene.
  auto entity = scene.CreateEntity();
  scene.AddComponentToEntity<TestComponentA>(entity);
  scene.OperateSystems(0);

  // Remove the Entity and update the Scene.
  scene.RemoveEntity(entity);
  scene.OperateSystems(0);

  assert(!scene.DoesEntityHaveComponent<TestComponentA>(0));
}

/******************************************************************************/
inline void TestEntityQuery()
{
  // Create a Scene and register a few components.
  Scene scene;
  scene.RegisterComponentType<TestComponentA>();
  scene.RegisterComponentType<TestComponentB>();

  // Create an Entity and add some components to it.
  auto entity = scene.CreateEntity();
  scene.AddComponentToEntity<TestComponentA>(entity);
  scene.AddComponentToEntity<TestComponentB>(entity);

  // Create some dummy Entities.
  scene.CreateEntity();
  scene.CreateEntity();
  scene.CreateEntity();

  // Update the Scene.
  scene.OperateSystems(0);

  // Create a Siganture to query with.
  auto signature = scene.CreateSignature();
  signature[scene.GetComponentIndex<TestComponentA>()] = true;
  signature[scene.GetComponentIndex<TestComponentB>()] = true;
  auto entities = scene.GetEntitiesWithSignature(signature);

  assert(entities.size() == 1);
  assert(entities[0] == entity);
}

/******************************************************************************/
inline void TestSignatureRelevancyCheck()
{
  Signature signatureA { false, false, true, false, true, false };
  Signature signatureB { true, true, true, false, true, false };

  assert(IsSignatureRelevant(signatureB, signatureA));

  signatureB[2] = false;

  assert(!IsSignatureRelevant(signatureB, signatureA));
}

} // namespace Kuma3D

#endif
