#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP

#include <Scene.hpp>

#include <RenderSystem.hpp>

#include <ModelLoader.hpp>
#include <ShaderLoader.hpp>

#include <Mesh.hpp>
#include <Transform.hpp>

#include "BulletSystem.hpp"
#include "ShipControlSystem.hpp"
#include "ShipControl.hpp"

namespace StarBear {

inline std::unique_ptr<Kuma3D::Scene> CreateScene()
{
  auto scene = std::make_unique<Kuma3D::Scene>();

  scene->AddSystem(std::make_unique<BulletSystem>());
  scene->AddSystem(std::make_unique<ShipControlSystem>());

  // Create and add a RenderSystem.
  auto renderSystem = std::make_unique<Kuma3D::RenderSystem>();
  renderSystem->CreateCamera(*scene);
  scene->AddSystem(std::move(renderSystem));

  // Create and add a ship.
  auto shipEntity = scene->CreateEntity();

  Kuma3D::Transform shipTransform;
  shipTransform.mPosition = Kuma3D::Vec3(0.0, 0.0, -45.0);
  scene->AddComponentToEntity<Kuma3D::Transform>(shipEntity, shipTransform);
  scene->AddComponentToEntity<ShipControl>(shipEntity);

  // Load the ship model and create an Entity for each Mesh.
  auto shaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/ShipShader.vert",
                                                            "resources/shaders/ShipShader.frag");

  auto modelID = Kuma3D::ModelLoader::LoadModel("resources/ship.obj");
  auto modelMeshes = Kuma3D::ModelLoader::GetModel(modelID);
  for(auto& mesh : modelMeshes)
  {
    // Create an entity for this mesh.
    auto meshEntity = scene->CreateEntity();

    // Add the Mesh and a Transform to the entity.
    mesh.mShaders.emplace_back(shaderID);
    scene->AddComponentToEntity<Kuma3D::Mesh>(meshEntity, mesh);

    Kuma3D::Transform meshTransform;
    meshTransform.mParent = shipEntity;
    meshTransform.mUseParent = true;
    scene->AddComponentToEntity<Kuma3D::Transform>(meshEntity, meshTransform);
  }

  return std::move(scene);
}

} // namespace StarBear

#endif
