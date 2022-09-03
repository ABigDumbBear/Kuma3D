#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP

#include <Scene.hpp>

#include <RenderSystem.hpp>
#include <ShaderLoader.hpp>

#include <Mesh.hpp>
#include <Transform.hpp>

namespace Untitled {

inline Kuma3D::Mesh CreateShipMesh()
{
  Kuma3D::Mesh mesh;

  Kuma3D::MeshVertex vertex;
  vertex.mPosition = Kuma3D::Vec3(0.0, 0.0, 0.0);
  vertex.mColor = Kuma3D::Vec3(1.0, 1.0, 1.0);
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.0, 1.0);
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.0, 0.0, -1.0);
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.0, 1.0);
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(0);
  mesh.mIndices.emplace_back(1);
  mesh.mIndices.emplace_back(2);
  mesh.mIndices.emplace_back(0);
  mesh.mIndices.emplace_back(3);
  mesh.mIndices.emplace_back(2);

  auto shaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/ShipShader.vert",
                                                            "resources/shaders/ShipShader.frag");
  mesh.mShaders.emplace_back(shaderID);

  mesh.mRenderMode = Kuma3D::RenderMode::eLINE_LOOP;
  mesh.mDirty = true;

  return mesh;
}

inline std::unique_ptr<Kuma3D::Scene> CreateScene()
{
  auto scene = std::make_unique<Kuma3D::Scene>();

  // Create and add a RenderSystem.
  auto renderSystem = std::make_unique<Kuma3D::RenderSystem>();
  renderSystem->CreateCamera(*scene);
  scene->AddSystem(std::move(renderSystem));

  // Create and add a ship.
  auto shipEntity = scene->CreateEntity();
  auto shipMesh = CreateShipMesh();
  scene->AddComponentToEntity<Kuma3D::Mesh>(shipEntity, shipMesh);
  Kuma3D::Transform shipTransform;
  shipTransform.mPosition = Kuma3D::Vec3(0.0, 0.0, -5.0);
  shipTransform.mRotation = Kuma3D::Vec3(0.0, 0.0, 0.0);
  scene->AddComponentToEntity<Kuma3D::Transform>(shipEntity, shipTransform);

  return std::move(scene);
}

} // namespace Untitled

#endif
