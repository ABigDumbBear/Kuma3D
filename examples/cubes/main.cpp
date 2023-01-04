#include <memory>
#include <random>

#include <Entity.hpp>
#include <Game.hpp>
#include <Scene.hpp>

#include <RenderSystem.hpp>

#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include <Camera.hpp>
#include <Light.hpp>
#include <Mesh.hpp>
#include <Transform.hpp>

#include "Physics.hpp"
#include "PhysicsSystem.hpp"

/******************************************************************************/
Kuma3D::Mesh CreateCubeMesh()
{
  Kuma3D::Mesh mesh;
  Kuma3D::MeshVertex vertex;

  // Front face
  vertex.mPosition = Kuma3D::Vec3(0, 0, 0);
  vertex.mNormal = Kuma3D::Vec3(0, 0, -1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 1, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(0);
  mesh.mIndices.emplace_back(1);
  mesh.mIndices.emplace_back(3);
  mesh.mIndices.emplace_back(1);
  mesh.mIndices.emplace_back(2);
  mesh.mIndices.emplace_back(3);

  // Top face
  vertex.mPosition = Kuma3D::Vec3(0, 1, 0);
  vertex.mNormal = Kuma3D::Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 1, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 1, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 1, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(4);
  mesh.mIndices.emplace_back(5);
  mesh.mIndices.emplace_back(7);
  mesh.mIndices.emplace_back(5);
  mesh.mIndices.emplace_back(6);
  mesh.mIndices.emplace_back(7);

  // Back face
  vertex.mPosition = Kuma3D::Vec3(0, 0, 1);
  vertex.mNormal = Kuma3D::Vec3(0, 0, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 0, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 1, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 1, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(8);
  mesh.mIndices.emplace_back(9);
  mesh.mIndices.emplace_back(11);
  mesh.mIndices.emplace_back(9);
  mesh.mIndices.emplace_back(10);
  mesh.mIndices.emplace_back(11);

  // Left face
  vertex.mPosition = Kuma3D::Vec3(0, 0, 1);
  vertex.mNormal = Kuma3D::Vec3(-1, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 0, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 1, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(12);
  mesh.mIndices.emplace_back(13);
  mesh.mIndices.emplace_back(15);
  mesh.mIndices.emplace_back(13);
  mesh.mIndices.emplace_back(14);
  mesh.mIndices.emplace_back(15);

  // Bottom face
  vertex.mPosition = Kuma3D::Vec3(0, 0, 1);
  vertex.mNormal = Kuma3D::Vec3(0, -1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 0, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(16);
  mesh.mIndices.emplace_back(17);
  mesh.mIndices.emplace_back(19);
  mesh.mIndices.emplace_back(17);
  mesh.mIndices.emplace_back(18);
  mesh.mIndices.emplace_back(19);

  // Right face
  vertex.mPosition = Kuma3D::Vec3(1, 0, 0);
  vertex.mNormal = Kuma3D::Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 0, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 1, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(1, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
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

/******************************************************************************/
Kuma3D::Transform CreateRandomTransform(std::random_device& aDevice)
{
  Kuma3D::Transform transform;

  std::mt19937 generator(aDevice());
  std::uniform_real_distribution<> dist(-50, 50);

  transform.mPosition.x = dist(generator);
  transform.mPosition.y = dist(generator);
  transform.mPosition.z = dist(generator) - 70;

  return transform;
}

/******************************************************************************/
int main(int argc, char* argv[])
{
  Kuma3D::WindowOptions options;
  options.mWidth = 1280;
  options.mHeight = 720;
  options.mTitle = "Cubes";

  // Initialize the game window and load the cube shader and texture.
  Kuma3D::Game::Initialize(options);
  auto shaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/CubeShader.vert",
                                                            "resources/shaders/CubeShader.frag");
  auto lightShaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/LightShader.vert",
                                                                 "resources/shaders/LightShader.frag");
  auto textureID = Kuma3D::TextureLoader::LoadTextureFromFile("resources/texture.png",
                                                              Kuma3D::TextureStorageFormat::eRGBA,
                                                              Kuma3D::TextureWrapOption::eREPEAT,
                                                              Kuma3D::TextureFilterOption::eNEAREST);

  // Create a scene, a camera, and a bunch of cubes.
  auto scene = std::make_unique<Kuma3D::Scene>();

  int numCubes = 5000;
  if(argc >= 2)
  {
    numCubes = std::atoi(argv[1]);
  }
  scene->RegisterComponentType<Kuma3D::Camera>(1);
  scene->RegisterComponentType<Kuma3D::Light>(1);
  scene->RegisterComponentType<Kuma3D::Transform>(numCubes + 2);
  scene->RegisterComponentType<Kuma3D::Mesh>(numCubes + 1);
  scene->RegisterComponentType<Cubes::Physics>(numCubes + 1);

  auto camera = scene->CreateEntity();
  scene->AddComponentToEntity<Kuma3D::Camera>(camera);
  scene->AddComponentToEntity<Kuma3D::Transform>(camera);

  std::random_device rd;
  for(int i = 0; i < numCubes; ++i)
  {
    auto cube = scene->CreateEntity();

    auto transform = CreateRandomTransform(rd);
    scene->AddComponentToEntity<Kuma3D::Transform>(cube, transform);

    auto mesh = CreateCubeMesh();
    mesh.mShaders.emplace_back(shaderID);
    mesh.mTextures.emplace_back(textureID);
    scene->AddComponentToEntity<Kuma3D::Mesh>(cube, mesh);

    Cubes::Physics physics;
    physics.mAcceleration.y = -9.81;
    //scene->AddComponentToEntity<Cubes::Physics>(cube, physics);
  }

  // Create a light.
  auto light = scene->CreateEntity();
  Kuma3D::Transform lightTransform;
  lightTransform.mPosition = Kuma3D::Vec3(0, 0, -25);
  scene->AddComponentToEntity<Kuma3D::Transform>(light, lightTransform);
  scene->AddComponentToEntity<Kuma3D::Light>(light);
  Cubes::Physics lightPhysics;
  lightPhysics.mAcceleration.z = -2;
  lightPhysics.mAcceleration.x = -2;
  scene->AddComponentToEntity<Cubes::Physics>(light, lightPhysics);
  auto lightMesh = CreateCubeMesh();
  lightMesh.mShaders.emplace_back(lightShaderID);
  scene->AddComponentToEntity<Kuma3D::Mesh>(light, lightMesh);

  // Add a PhysicsSystem to move the cubes, and a RenderSystem to draw them.
  scene->AddSystem(std::make_unique<Cubes::PhysicsSystem>());
  scene->AddSystem(std::make_unique<Kuma3D::RenderSystem>());

  // Set the scene and run the game.
  Kuma3D::Game::SetScene(std::move(scene));
  Kuma3D::Game::Run();

  // Unload all resources.
  Kuma3D::ShaderLoader::UnloadShaders();
  Kuma3D::TextureLoader::UnloadTextures();
  Kuma3D::Game::Uninitialize();

  return 0;
}
