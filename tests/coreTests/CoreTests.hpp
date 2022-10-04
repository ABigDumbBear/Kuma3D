#ifndef CORETESTS_HPP
#define CORETESTS_HPP

#include <cassert>

#include <ComponentList.hpp>
#include <Scene.hpp>
#include <Types.hpp>

namespace Kuma3D {

struct TestComponent { int mValue { 0 }; };

inline void TestComponentListAddition()
{
  ComponentListT<TestComponent> list;

  TestComponent component { 42 };
  list.AddComponentToEntity(0, component);

  bool accessFailed = false;
  try
  {
    auto& c = list.GetComponentForEntity(0);
    assert(c.mValue == 42);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }

  assert(!accessFailed);
}

inline void TestComponentListRemoval()
{
  ComponentListT<TestComponent> list;

  TestComponent component { 42 };
  list.AddComponentToEntity(0, component);
  list.RemoveComponentFromEntity(0);

  bool accessFailed = false;
  try
  {
    list.GetComponentForEntity(0);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }

  assert(accessFailed);
}

inline void TestEntityRemoval()
{
  Scene scene;
  scene.RegisterComponentType<TestComponent>();

  auto entity = scene.CreateEntity();
  scene.AddComponentToEntity<TestComponent>(entity);
  scene.RemoveEntity(entity);

  scene.OperateSystems(0.0);

  bool accessFailed = false;
  try
  {
    scene.GetComponentForEntity<TestComponent>(entity);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }

  assert(accessFailed);
}

} // namespace Kuma3D

#endif
