#ifndef CORETESTS_HPP
#define CORETESTS_HPP

#include <cassert>

#include <ComponentList.hpp>
#include <Scene.hpp>
#include <Types.hpp>

#include <Transform.hpp>

namespace Kuma3D {

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

inline void TestEntityRemoval()
{
  Scene scene;
  scene.RegisterComponentType<Transform>();

  auto entity = scene.CreateEntity();
  scene.AddComponentToEntity<Transform>(entity);
  scene.RemoveEntity(entity);

  scene.OperateSystems(0.0);

  bool accessFailed = false;
  try
  {
    auto& transform = scene.GetComponentForEntity<Transform>(entity);
  }
  catch(const std::invalid_argument& e)
  {
    accessFailed = true;
  }

  assert(accessFailed);
}

} // namespace Kuma3D

#endif
