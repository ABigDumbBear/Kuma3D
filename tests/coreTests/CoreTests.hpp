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
  ComponentListT<TestComponentA> list(5);
  TestComponentA component;
  component.mValue = 55;

  list.AddComponentToEntity(0, component);

  auto& test = list.GetComponentForEntity(0);
  assert(test.mValue == 55);
}

/******************************************************************************/
inline void TestComponentListRemoval()
{
  ComponentListT<TestComponentA> list(5);

  TestComponentA transform;
  list.AddComponentToEntity(0, transform);
  list.RemoveComponentFromEntity(0);

  bool accessFailed = false;
  try
  {
    transform = list.GetComponentForEntity(0);
  }
  catch(const std::out_of_range& e)
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

  assert(scene.IsEntityScheduledForRemoval(entity));
  scene.OperateSystems(0);
  assert(!scene.IsEntityScheduledForRemoval(entity));
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
