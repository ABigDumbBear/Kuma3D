#ifndef BLOCKUTIL_HPP
#define BLOCKUTIL_HPP

#include "Block.hpp"

#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include <Mesh.hpp>

namespace KumaTetris {

/**
 * Creates and returns a random Block.
 *
 * @return A random Block.
 */
inline Block CreateBlock()
{
  Block block;

  int blockType = rand() % 7;
  if(blockType == 0)
  {
    block = IBlock;
  }
  else if(blockType == 1)
  {
    block = JBlock;
  }
  else if(blockType == 2)
  {
    block = LBlock;
  }
  else if(blockType == 3)
  {
    block = OBlock;
  }
  else if(blockType == 4)
  {
    block = SBlock;
  }
  else if(blockType == 5)
  {
    block = TBlock;
  }
  else if(blockType == 6)
  {
    block = ZBlock;
  }

  block.mPosition.x = 3;
  block.mPosition.y = 25;

  return block;
}

/**
 * Creates and returns a cube mesh for a given size.
 *
 * @param aSize The cube size in pixels.
 * @param aColor The color of the cube.
 * @return A cube mesh for the given size.
 */
inline Kuma3D::Mesh CreateCube(int aSize, const Kuma3D::Vec3& aColor)
{
  Kuma3D::Mesh mesh;

  float xVal = aSize / 2.0;
  float yVal = aSize / 2.0;
  float zVal = aSize / 2.0;

  Kuma3D::MeshVertex vertex;

  // Vertices for each face are defined in the following order:
  // 1) bottom left
  // 2) bottom right
  // 3) top right
  // 4) top left

  // Front face
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, zVal);
  vertex.mColor = aColor;
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, zVal);
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
  vertex.mTexCoords[0] = 0.0;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, zVal);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, -zVal);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, -zVal);
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
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, -zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, -zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, -zVal);
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
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, yVal, -zVal);
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
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, -zVal);
  vertex.mTexCoords[0] = 1.0;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, zVal);
  vertex.mTexCoords[0] = 1.0;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(-xVal, -yVal, zVal);
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
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, -yVal, zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 0.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, zVal);
  vertex.mTexCoords[0] = 0.66;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Kuma3D::Vec3(xVal, yVal, -zVal);
  vertex.mTexCoords[0] = 0.33;
  vertex.mTexCoords[1] = 1.0;
  mesh.mVertices.emplace_back(vertex);

  mesh.mIndices.emplace_back(20);
  mesh.mIndices.emplace_back(21);
  mesh.mIndices.emplace_back(23);
  mesh.mIndices.emplace_back(21);
  mesh.mIndices.emplace_back(22);
  mesh.mIndices.emplace_back(23);

  // Add the shader.
  auto shaderID = Kuma3D::ShaderLoader::LoadShaderFromFiles("resources/shaders/TileShader.vert",
                                                            "resources/shaders/TileShader.frag");
  mesh.mShaders.emplace_back(shaderID);

  // Add the texture.
  auto textureID = Kuma3D::TextureLoader::LoadTextureFromFile("resources/tileTexture.png");
  mesh.mTextures.emplace_back(textureID);

  mesh.mSystem = Kuma3D::CoordinateSystem::eSCREEN_SPACE;
  mesh.mDirty = true;

  return mesh;
}
} // namespace KumaTetris

#endif
