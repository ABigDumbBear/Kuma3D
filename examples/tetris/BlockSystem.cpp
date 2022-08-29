#include "BlockSystem.hpp"

#include <Scene.hpp>

#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include <Mesh.hpp>
#include <Transform.hpp>

#include "Block.hpp"

namespace Tetris {

const float BlockSystem::mFallSpeed = 0.1;

/**
 * Creates and returns a random Block.
 *
 * @return A random Block.
 */
Block CreateBlock()
{
  Block block = SBlock;
  block.mPosition.x = 3;
  block.mPosition.y = 25;

  return block;
}

/**
 * Creates and returns a cube mesh for a given size.
 *
 * @param aSize The cube size in pixels.
 * @return A cube mesh for the given size.
 */
Kuma3D::Mesh CreateCube(int aSize)
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
  vertex.mColor = Kuma3D::Vec3(1.0, 0.0, 0.0);
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

/******************************************************************************/
void BlockSystem::Initialize(Kuma3D::Scene& aScene)
{
  // Register the relevant components.
  if(!aScene.IsComponentTypeRegistered<Kuma3D::Mesh>())
  {
    aScene.RegisterComponentType<Kuma3D::Mesh>();
  }

  if(!aScene.IsComponentTypeRegistered<Kuma3D::Transform>())
  {
    aScene.RegisterComponentType<Kuma3D::Transform>();
  }

  if(!aScene.IsComponentTypeRegistered<Block>())
  {
    aScene.RegisterComponentType<Block>();
  }

  // Set the signature to care about Entities with Block components.
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Block>()] = true;
  SetSignature(signature);

  // Connect to the relevant signals.
  Kuma3D::KeyPressed.Connect(mObserver, [this](const Kuma3D::KeyCode& aKey,
                                               int aMods)
  {
    this->HandleKeyPressed(aKey, aMods);
  });

  // Create the first block.
  auto blockEntity = aScene.CreateEntity();
  auto newBlock = CreateBlock();
  aScene.AddComponentToEntity<Block>(blockEntity, newBlock);
}

/******************************************************************************/
void BlockSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  // For each new Entity, retrieve the Block component and create 4 new
  // Entities, each with a cube mesh and a transform component, to represent
  // the tiles in the block.
  for(const auto& entity : mNewEntities)
  {
    auto& block = aScene.GetComponentForEntity<Block>(entity);
    auto& tiles = block.mTiles;
    auto& column = block.mPosition.x;
    auto& row = block.mPosition.y;

    for(int i = 0; i < 4; ++i)
    {
      auto tileEntity = aScene.CreateEntity();

      auto cubeMesh = CreateCube(mTileSizeInPixels);
      Kuma3D::Transform cubeTransform;
      cubeTransform.mPosition.x = (mTileSizeInPixels * (column + tiles[i].x)) + (mTileSizeInPixels / 2.0);
      cubeTransform.mPosition.y = (mTileSizeInPixels * (row + tiles[i].y)) + (mTileSizeInPixels / 2.0);

      aScene.AddComponentToEntity<Kuma3D::Mesh>(tileEntity, cubeMesh);
      aScene.AddComponentToEntity<Kuma3D::Transform>(tileEntity, cubeTransform);

      mEntityTileMap[entity][i] = tileEntity;
    }
  }
  mNewEntities.clear();

  // Handle moving the tiles of each block down/left/right, and update
  // the corresponding transforms.
  auto dt = aTime - mTimeSinceLastFall;
  for(const auto& blockEntity : GetEntities())
  {
    auto& block = aScene.GetComponentForEntity<Block>(blockEntity);
    auto& tiles = block.mTiles;
    auto& column = block.mPosition.x;
    auto& row = block.mPosition.y;

    // Move the Block left or right, depending on the previously pressed key.
    switch(mKeyPress)
    {
      case Kuma3D::KeyCode::eKEY_LEFT:
      {
        column -= 1;
        break;
      }
      case Kuma3D::KeyCode::eKEY_RIGHT:
      {
        column += 1;
        break;
      }
      default:
      {
        break;
      }
    }

    // Move the Block down if enough time has passed.
    bool blockFallen = false;
    if(dt >= mFallSpeed)
    {
      row -= 1;

      // If the block is at the bottom of the grid, or if there's a collision,
      // then the block has finished falling.
      if(row == 0 || IsBlockColliding(block))
      {
        blockFallen = true;
      }
    }

    // For each tile in the Block, update the position of the corresponding
    // 3D cube.
    for(int i = 0; i < 4; ++i)
    {
      auto tileEntity = mEntityTileMap[blockEntity][i];
      auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(tileEntity);
      transform.mPosition.x = (mTileSizeInPixels * (column + tiles[i].x)) + (mTileSizeInPixels / 2.0);
      transform.mPosition.y = (mTileSizeInPixels * (row  + tiles[i].y)) + (mTileSizeInPixels / 2.0);
    }

    // If the block has fallen, add each tile to the list of fallen
    // tiles, then remove the Block component and create a new block.
    if(blockFallen)
    {
      for(const auto& tile : tiles)
      {
        GridPosition tilePos = tile;
        tilePos.x += column;
        tilePos.y += row;

        mFallenTiles.emplace_back(tilePos);
      }

      aScene.RemoveComponentFromEntity<Block>(blockEntity);
      mEntityTileMap.erase(blockEntity);

      auto newEntity = aScene.CreateEntity();
      auto newBlock = CreateBlock();
      aScene.AddComponentToEntity<Block>(newEntity, newBlock);
    }
  }

  // Reset the fall timer.
  if(dt >= mFallSpeed)
  {
    mTimeSinceLastFall = aTime;
  }

  // Reset the previously pressed key.
  mKeyPress = Kuma3D::KeyCode::eKEY_UNKNOWN;
}

/******************************************************************************/
void BlockSystem::HandleEntityBecameEligible(const Kuma3D::Entity& aEntity)
{
  mNewEntities.emplace_back(aEntity);
}

/******************************************************************************/
void BlockSystem::HandleKeyPressed(const Kuma3D::KeyCode& aKey, int aMods)
{
  mKeyPress = aKey;
}

/******************************************************************************/
bool BlockSystem::IsBlockColliding(const Block& aBlock)
{
  bool colliding = false;

  for(const auto& blockTile : aBlock.mTiles)
  {
    GridPosition tilePos;
    tilePos.x = blockTile.x + aBlock.mPosition.x;
    tilePos.y = blockTile.y + aBlock.mPosition.y;

    for(const auto& fallenTile : mFallenTiles)
    {
      if(fallenTile.x == tilePos.x &&
         fallenTile.y == tilePos.y - 1)
      {
        colliding = true;
        break;
      }
    }

    if(colliding)
    {
      break;
    }
  }

  return colliding;
}

} // namespace Tetris
