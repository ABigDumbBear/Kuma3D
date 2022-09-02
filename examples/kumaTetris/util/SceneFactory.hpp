#ifndef SCENEFACTORY_HPP
#define SCENEFACTORY_HPP

#include <Scene.hpp>
#include <Types.hpp>

#include <AudioSystem.hpp>
#include <RenderSystem.hpp>

#include <AudioLoader.hpp>
#include <ShaderLoader.hpp>

#include <Audio.hpp>
#include <Camera.hpp>
#include <Mesh.hpp>
#include <Transform.hpp>

#include "BlockSystem.hpp"
#include "MovementSystem.hpp"

namespace KumaTetris {

/**
 * Creates a mesh for a 2D grid of the given dimensions.
 *
 * @param aWidth The width of the grid.
 * @param aHeight The height of the grid.
 * @param aSize The size (in pixels) of one grid square.
 * @return A mesh for the grid.
 */
inline Kuma3D::Mesh CreateGrid(int aWidth, int aHeight, int aSize)
{
  Kuma3D::Mesh mesh;

  for(int column = 0; column < aWidth; ++column)
  {
    for(int row = 0; row < aHeight; ++row)
    {
      // Create a square for each space in the grid.
      Kuma3D::MeshVertex vertex;

      int xVal = column * aSize;
      int yVal = row * aSize;

      // Bottom Left
      vertex.mPosition = Kuma3D::Vec3(xVal, yVal, 0.0);
      vertex.mColor = Kuma3D::Vec3(1.0, 1.0, 1.0);
      mesh.mVertices.emplace_back(vertex);

      // Bottom Right
      vertex.mPosition = Kuma3D::Vec3(xVal + aSize, yVal, 0.0);
      vertex.mColor = Kuma3D::Vec3(1.0, 1.0, 1.0);
      mesh.mVertices.emplace_back(vertex);

      // Top Right
      vertex.mPosition = Kuma3D::Vec3(xVal + aSize, yVal + aSize, 0.0);
      vertex.mColor = Kuma3D::Vec3(1.0, 1.0, 1.0);
      mesh.mVertices.emplace_back(vertex);

      // Top Left
      vertex.mPosition = Kuma3D::Vec3(xVal, yVal + aSize, 0.0);
      vertex.mColor = Kuma3D::Vec3(1.0, 1.0, 1.0);
      mesh.mVertices.emplace_back(vertex);

      // Add the indices.
      int numSquares = row + (column * aHeight);
      int startIndex = numSquares * 4;
      mesh.mIndices.emplace_back(startIndex);
      mesh.mIndices.emplace_back(startIndex + 1);
      mesh.mIndices.emplace_back(startIndex + 2);
      mesh.mIndices.emplace_back(startIndex + 3);
    }

    // After adding the vertices and indices for a column, the next index
    // should be the vertex at the bottom left of the column to avoid
    // drawing lines across existing squares.
    mesh.mIndices.emplace_back(column * (aHeight * 4));
  }

  mesh.mRenderMode = Kuma3D::RenderMode::eLINE_STRIP;
  mesh.mSystem = Kuma3D::CoordinateSystem::eSCREEN_SPACE;
  mesh.mDirty = true;

  return mesh;
}

/**
 * Creates and returns the main scene.
 *
 * @return A scene for the game.
 */
inline std::unique_ptr<Kuma3D::Scene> CreateScene()
{
  auto newScene = std::make_unique<Kuma3D::Scene>();

  // Create and add a BlockSystem.
  newScene->AddSystem(std::make_unique<BlockSystem>());

  // Create and add a MovementSystem
  newScene->AddSystem(std::make_unique<MovementSystem>());

  // Create and add an AudioSystem.
  newScene->AddSystem(std::make_unique<Kuma3D::AudioSystem>());

  // Create and add a RenderSystem.
  auto renderSystem = std::make_unique<Kuma3D::RenderSystem>();

  // Create a camera to render with.
  auto cameraEntity = renderSystem->CreateCamera(*newScene.get());

  // Set the camera viewport to fit the grid.
  auto& cameraInfo = newScene->GetComponentForEntity<Kuma3D::Camera>(cameraEntity);
  cameraInfo.mViewportX = 320;
  cameraInfo.mViewportY = 800;
  newScene->AddSystem(std::move(renderSystem));

  // Create the grid.
  auto shaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/GridShader.vert",
                                                            "resources/shaders/GridShader.frag");
  auto gridEntity = newScene->CreateEntity();
  auto gridMesh = CreateGrid(10, 25, 32);
  gridMesh.mShaders.emplace_back(shaderID);
  newScene->AddComponentToEntity<Kuma3D::Mesh>(gridEntity, gridMesh);
  newScene->AddComponentToEntity<Kuma3D::Transform>(gridEntity);

  // Create an audio player for the background music.
  auto bgmID = Kuma3D::AudioLoader::LoadAudioFromFile("resources/BGM.wav");

  auto bgmEntity = newScene->CreateEntity();
  Kuma3D::Audio bgm;
  bgm.mSoundID = bgmID;
  bgm.mVolume = 0.5;
  bgm.mLooping = true;
  newScene->AddComponentToEntity<Kuma3D::Audio>(bgmEntity, bgm);

  return std::move(newScene);
}

} // namespace KumaTetris

#endif
