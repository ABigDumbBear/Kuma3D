#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP

#include <Scene.hpp>

#include <RenderSystem.hpp>

#include <ModelLoader.hpp>
#include <ShaderLoader.hpp>

#include <Mesh.hpp>
#include <Transform.hpp>

namespace Untitled {

inline std::unique_ptr<Kuma3D::Scene> CreateScene()
{
  auto scene = std::make_unique<Kuma3D::Scene>();

  // Create and add a RenderSystem.
  auto renderSystem = std::make_unique<Kuma3D::RenderSystem>();
  renderSystem->CreateCamera(*scene);
  scene->AddSystem(std::move(renderSystem));

  // Create and add a ship.
  auto shipEntity = scene->CreateEntity();

  Kuma3D::Transform shipTransform;
  shipTransform.mPosition = Kuma3D::Vec3(0.0, 0.0, -25.0);
  shipTransform.mRotation = Kuma3D::Vec3(25.0, 35.0, 17.0);
  scene->AddComponentToEntity<Kuma3D::Transform>(shipEntity, shipTransform);

  // Load the ship model and create an Entity for each Mesh.
  auto shaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/ShipShader.vert",
                                                            "resources/shaders/ShipShader.frag");

  auto modelID = Kuma3D::ModelLoader::LoadModel("resources/StarShip.obj");
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

} // namespace Untitled

#endif
