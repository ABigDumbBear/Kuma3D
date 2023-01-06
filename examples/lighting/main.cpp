#include <Game.hpp>
#include <Scene.hpp>

#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include <Camera.hpp>
#include <Light.hpp>
#include <Mesh.hpp>
#include <Transform.hpp>

#include <RenderSystem.hpp>

#include "Orbit.hpp"
#include "OrbitSystem.hpp"

/******************************************************************************/
Kuma3D::Mesh CreateCubeMesh()
{
  Kuma3D::Mesh mesh;
  Kuma3D::MeshVertex vertex;

  // Front face
  vertex.mPosition = Kuma3D::Vec3(-0.5, -0.5, -0.5);
  vertex.mNormal = Kuma3D::Vec3(0, 0, -1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, -0.5, -0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.5, -0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.5, -0.5);
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
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.5, -0.5);
  vertex.mNormal = Kuma3D::Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.5, -0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.5, 0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.5, 0.5);
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
  vertex.mPosition = Kuma3D::Vec3(-0.5, -0.5, 0.5);
  vertex.mNormal = Kuma3D::Vec3(0, 0, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, -0.5, 0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.5, 0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.5, 0.5);
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
  vertex.mPosition = Kuma3D::Vec3(-0.5, -0.5, 0.5);
  vertex.mNormal = Kuma3D::Vec3(-1, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, -0.5, -0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.5, -0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, 0.5, 0.5);
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
  vertex.mPosition = Kuma3D::Vec3(-0.5, -0.5, 0.5);
  vertex.mNormal = Kuma3D::Vec3(0, -1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, -0.5, 0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, -0.5, -0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-0.5, -0.5, -0.5);
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
  vertex.mPosition = Kuma3D::Vec3(0.5, -0.5, -0.5);
  vertex.mNormal = Kuma3D::Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, -0.5, 0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.5, 0.5);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(0.5, 0.5, -0.5);
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
int main()
{
  Kuma3D::WindowOptions options;
  options.mWidth = 1280;
  options.mHeight = 720;
  options.mTitle = "Lighting";

  // Initialize the game window and load the cube shader and texture.
  Kuma3D::Game::Initialize(options);
  auto cubeShaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/CubeShader.vert",
                                                                "resources/shaders/CubeShader.frag");
  auto lightShaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/LightShader.vert",
                                                                 "resources/shaders/LightShader.frag");
  auto textureID = Kuma3D::TextureLoader::LoadTextureFromFile("resources/texture.png",
                                                              Kuma3D::TextureStorageFormat::eRGBA,
                                                              Kuma3D::TextureWrapOption::eREPEAT,
                                                              Kuma3D::TextureFilterOption::eNEAREST);

  // Create a scene, a camera, a cube, and a light source.
  auto scene = std::make_unique<Kuma3D::Scene>();
  scene->RegisterComponentType<Kuma3D::Camera>(1);
  scene->RegisterComponentType<Kuma3D::Light>(1);
  scene->RegisterComponentType<Kuma3D::Mesh>(2);
  scene->RegisterComponentType<Kuma3D::Transform>(3);
  scene->RegisterComponentType<Lighting::Orbit>(1);

  auto camera = scene->CreateEntity();
  scene->AddComponentToEntity<Kuma3D::Transform>(camera);
  scene->AddComponentToEntity<Kuma3D::Camera>(camera);

  auto cube = scene->CreateEntity();
  Kuma3D::Transform cubeTransform;
  cubeTransform.mPosition.z = -10;
  cubeTransform.mRotation.x = 45;
  cubeTransform.mRotation.y = 45;
  cubeTransform.mRotation.z = 45;
  scene->AddComponentToEntity<Kuma3D::Transform>(cube, cubeTransform);
  auto cubeMesh = CreateCubeMesh();
  cubeMesh.mShaders.emplace_back(cubeShaderID);
  cubeMesh.mTextures.emplace_back(textureID);
  scene->AddComponentToEntity<Kuma3D::Mesh>(cube, cubeMesh);

  auto light = scene->CreateEntity();
  Kuma3D::Transform lightTransform;
  lightTransform.mPosition = Kuma3D::Vec3(0, 0, -25);
  scene->AddComponentToEntity<Kuma3D::Transform>(light, lightTransform);
  auto lightMesh = CreateCubeMesh();
  lightMesh.mShaders.emplace_back(lightShaderID);
  scene->AddComponentToEntity<Kuma3D::Mesh>(light, lightMesh);
  scene->AddComponentToEntity<Kuma3D::Light>(light);
  Lighting::Orbit orbit;
  orbit.mSpeed = 100;
  orbit.mAxis.y = 1;
  orbit.mRadius = 1.5;
  scene->AddComponentToEntity<Lighting::Orbit>(light, orbit);

  // Add an OrbitSystem to move the light and a RenderSystem to draw the scene.
  scene->AddSystem(std::make_unique<Lighting::OrbitSystem>());
  scene->AddSystem(std::make_unique<Kuma3D::RenderSystem>());

  Kuma3D::Game::SetScene(std::move(scene));
  Kuma3D::Game::Run();

  // Unload all resources.
  Kuma3D::ShaderLoader::UnloadShaders();
  Kuma3D::TextureLoader::UnloadTextures();
  Kuma3D::Game::Uninitialize();

  return 0;
}

