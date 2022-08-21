#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP

#include <cstdlib>
#include <ctime>

#include <Scene.hpp>

#include <Camera.hpp>
#include <Mesh.hpp>
#include <Transform.hpp>

#include <RenderSystem.hpp>

#include <Types.hpp>

#include "Movable.hpp"

#include "MovementSystem.hpp"

namespace example_2 {

inline Kuma3D::Mesh CreateCube()
{
  float xVal = 0.5;
  float yVal = 0.5;
  float zVal = 0.5;

  Kuma3D::Mesh mesh;

  // Create the 3D vertices for this cube.
  Kuma3D::MeshVertex vertex;

  // Vertices for each face are defined in the following order:
  // 1) bottom left
  // 2) bottom right
  // 3) top right
  // 4) top left

  // Front face
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(0);
  mesh.mIndices.emplace_back(1);
  mesh.mIndices.emplace_back(3);
  mesh.mIndices.emplace_back(1);
  mesh.mIndices.emplace_back(2);
  mesh.mIndices.emplace_back(3);

  // Top face
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.0;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.0;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(4);
  mesh.mIndices.emplace_back(5);
  mesh.mIndices.emplace_back(7);
  mesh.mIndices.emplace_back(5);
  mesh.mIndices.emplace_back(6);
  mesh.mIndices.emplace_back(7);

  // Back face
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(8);
  mesh.mIndices.emplace_back(9);
  mesh.mIndices.emplace_back(11);
  mesh.mIndices.emplace_back(9);
  mesh.mIndices.emplace_back(10);
  mesh.mIndices.emplace_back(11);

  // Left face
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(12);
  mesh.mIndices.emplace_back(13);
  mesh.mIndices.emplace_back(15);
  mesh.mIndices.emplace_back(13);
  mesh.mIndices.emplace_back(14);
  mesh.mIndices.emplace_back(15);

  // Bottom face
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords[0] = 1.0;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords[0] = 1.0;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(16);
  mesh.mIndices.emplace_back(17);
  mesh.mIndices.emplace_back(19);
  mesh.mIndices.emplace_back(17);
  mesh.mIndices.emplace_back(18);
  mesh.mIndices.emplace_back(19);

  // Right face
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 1.0, 0.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, zVal);
  vertex.mColor = Kuma3D::Vec3(0.0, 0.0, 1.0);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, -zVal);
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(20);
  mesh.mIndices.emplace_back(21);
  mesh.mIndices.emplace_back(23);
  mesh.mIndices.emplace_back(21);
  mesh.mIndices.emplace_back(22);
  mesh.mIndices.emplace_back(23);

  mesh.mDirty = true;

  return mesh;
}

inline std::unique_ptr<Kuma3D::Scene> CreateScene(const Kuma3D::ID& aCubeTextureID,
                                                        const Kuma3D::ID& aCubeShaderID)
{
  auto scene = std::make_unique<Kuma3D::Scene>();

  // Create a MovementSystem and add it to the scene.
  scene->AddSystem(std::make_unique<example_2::MovementSystem>());

  // Create a RenderSystem and add it to the scene.
  // IMPORTANT: Note that the RenderSystem is added last. This is because
  // we want all non-rendering logic to occur first, before rendering
  // the scene.
  auto renderSystem = std::make_unique<Kuma3D::RenderSystem>();
  renderSystem->CreateCamera(*scene.get());
  scene->AddSystem(std::move(renderSystem));

  // Create a bunch of new Entities and give each a Mesh, a Transform, and a
  // Movable component.
  //
  // These entities will automatically be tracked and rendered by
  // the RenderSystem. They will also be updated by the MovementSystem.
  srand(time(NULL));
  int xMax = 25;
  int yMax = 100;
  int zMax = 50;
  int speedMax = 5;

  for(int i = 0; i < 1000; ++i)
  {
    auto entity = scene->CreateEntity();

    // Create the Mesh.
    auto cubeMesh = CreateCube();
    cubeMesh.mTextures.emplace_back(aCubeTextureID);
    cubeMesh.mShaders.emplace_back(aCubeShaderID);
    scene->AddComponentToEntity<Kuma3D::Mesh>(entity, cubeMesh);

    // Create a Transform and randomize the position.
    Kuma3D::Transform transform;
    transform.mPosition.x = (rand() % xMax) * (rand() % 2 ? 1 : -1);
    transform.mPosition.y = (std::rand() % yMax) * (std::rand() % 2 ? 1 : -1);
    transform.mPosition.z = std::rand() % zMax * -1;
    scene->AddComponentToEntity<Kuma3D::Transform>(entity, transform);

    // Create a Movable component.
    example_2::Movable movableComponent;
    movableComponent.mSpeed = (std::rand() % speedMax) + 0.5;
    movableComponent.mDirection.y = -1.0;
    scene->AddComponentToEntity<example_2::Movable>(entity, movableComponent);
  }

  return std::move(scene);
}

} // namespace example_2

#endif
