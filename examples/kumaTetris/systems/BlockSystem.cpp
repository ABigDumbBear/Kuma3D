#include "BlockSystem.hpp"

#include <cstdlib>
#include <ctime>

#include <Scene.hpp>

#include <ShaderLoader.hpp>
#include <TextureLoader.hpp>

#include <Mesh.hpp>
#include <Transform.hpp>

#include "Block.hpp"
#include "BlockUtil.hpp"
#include "Movable.hpp"

namespace KumaTetris {

const float BlockSystem::mFallSpeed = 0.2;

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

  if(!aScene.IsComponentTypeRegistered<Movable>())
  {
    aScene.RegisterComponentType<Movable>();
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

  // Initialize the fallen tile grid.
  for(auto& row : mFallenTiles)
  {
    for(auto& tile : row)
    {
      tile = 0;
    }
  }

  // Create the first block.
  srand(time(NULL));
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
    auto& tiles = block.mRotations[block.mCurrentRotation];
    auto& column = block.mPosition.x;
    auto& row = block.mPosition.y;

    for(int i = 0; i < 4; ++i)
    {
      auto tileEntity = aScene.CreateEntity();

      auto cubeMesh = CreateCube(mTileSizeInPixels, block.mColor);
      Kuma3D::Transform cubeTransform;
      cubeTransform.mPosition.x = (mTileSizeInPixels * (column + tiles[i].x)) + (mTileSizeInPixels / 2.0);
      cubeTransform.mPosition.y = (mTileSizeInPixels * (row + tiles[i].y)) + (mTileSizeInPixels / 2.0);

      aScene.AddComponentToEntity<Kuma3D::Mesh>(tileEntity, cubeMesh);
      aScene.AddComponentToEntity<Kuma3D::Transform>(tileEntity, cubeTransform);
      aScene.AddComponentToEntity<Movable>(tileEntity);

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
    auto& column = block.mPosition.x;
    auto& row = block.mPosition.y;

    // Handle input, either moving the block left/right or rotating it.
    switch(mKeyPress)
    {
      case Kuma3D::KeyCode::eKEY_LEFT:
      {
        GridPosition newPos { column - 1, row };
        if(IsMoveValid(block, newPos))
        {
          column -= 1;
        }
        break;
      }
      case Kuma3D::KeyCode::eKEY_RIGHT:
      {
        GridPosition newPos { column + 1, row };
        if(IsMoveValid(block, newPos))
        {
          column += 1;
        }
        break;
      }
      case Kuma3D::KeyCode::eKEY_UP:
      {
        auto newRotation = block.mCurrentRotation + 1;
        if(newRotation > 3)
        {
          newRotation = 0;
        }

        if(IsRotationValid(block, newRotation))
        {
          block.mCurrentRotation = newRotation;
        }
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
      // If the block can move down 1 row, then do so. Otherwise,
      // the block has landed.
      if(row > 24)
      {
        --row;
      }
      else
      {
        GridPosition newPos { column, row - 1 };
        if(IsMoveValid(block, newPos))
        {
          block.mPosition = newPos;
        }
        else
        {
          blockFallen = true;
        }
      }
    }

    // For each tile in the Block, update the position of the corresponding 3D cube.
    auto& tiles = block.mRotations[block.mCurrentRotation];
    for(int i = 0; i < 4; ++i)
    {
      auto tileEntity = mEntityTileMap[blockEntity][i];
      auto& movable = aScene.GetComponentForEntity<Movable>(tileEntity);
      movable.mTargetPosition.x = (mTileSizeInPixels * (column + tiles[i].x)) + (mTileSizeInPixels / 2.0);
      movable.mTargetPosition.y = (mTileSizeInPixels * (row  + tiles[i].y)) + (mTileSizeInPixels / 2.0);
      movable.mMoveSpeed = 0.5;
      movable.mFinishedMoving = false;
    }

    // If the block has fallen, add each tile to the list of fallen
    // tiles, then remove the Block component and create a new block.
    if(blockFallen)
    {
      for(int i = 0; i < 4; ++i)
      {
        GridPosition tilePos = tiles[i];
        tilePos.x += column;
        tilePos.y += row;

        mFallenTiles[tilePos.y][tilePos.x] = 1;
        mFallenTileEntities[tilePos.y][tilePos.x] = mEntityTileMap[blockEntity][i];
      }

      aScene.RemoveEntity(blockEntity);
      mEntityTileMap.erase(blockEntity);

      auto newEntity = aScene.CreateEntity();
      auto newBlock = CreateBlock();
      aScene.AddComponentToEntity<Block>(newEntity, newBlock);

      // Check for any filled rows and remove them.
      RemoveFilledRows();

      // Update the tile Entities to their new positions after falling.
      UpdateFallenTileEntities(aScene);
    }
  }

  // Remove all Entities that have been marked for removal.
  for(const auto& entity : mEntitiesToRemove)
  {
    //aScene.RemoveEntity(entity);
    auto& movable = aScene.GetComponentForEntity<Movable>(entity);
    auto& transform = aScene.GetComponentForEntity<Kuma3D::Transform>(entity);

    movable.mTargetPosition = transform.mPosition;
    movable.mTargetPosition.x -= (10 * mTileSizeInPixels);
    movable.mMoveSpeed = 0.3;
    movable.mFinishedMoving = false;
  }
  mEntitiesToRemove.clear();

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

  for(const auto& blockTile : aBlock.mRotations[aBlock.mCurrentRotation])
  {
    GridPosition tilePos;
    tilePos.x = blockTile.x + aBlock.mPosition.x;
    tilePos.y = blockTile.y + aBlock.mPosition.y;

    if(tilePos.y > 0)
    {
      if(mFallenTiles[tilePos.y - 1][tilePos.x] == 1)
      {
        colliding = true;
        break;
      }
    }
  }

  return colliding;
}

/******************************************************************************/
bool BlockSystem::IsMoveValid(const Block& aBlock, const GridPosition& aPosition)
{
  bool success = true;

  // Check each tile at the new position to see if it's out of bounds or
  // if that space is already occupied.
  const auto& tiles = aBlock.mRotations[aBlock.mCurrentRotation];
  for(const auto& tile : tiles)
  {
    auto tileX = tile.x + aPosition.x;
    auto tileY = tile.y + aPosition.y;

    if(tileX < 0 || tileX > 9 || tileY < 0 || tileY > 24)
    {
      success = false;
      break;
    }
    else if(mFallenTiles[tileY][tileX] == 1)
    {
      success = false;
      break;
    }
  }

  return success;
}

/******************************************************************************/
bool BlockSystem::IsRotationValid(const Block& aBlock, int aRotation)
{
  bool success = true;

  // Check each tile at the new rotation to see if it's out of bounds or
  // if that space is already occupied.
  const auto& tiles = aBlock.mRotations[aRotation];
  const auto& position = aBlock.mPosition;
  for(const auto& tile : tiles)
  {
    auto tileX = tile.x + position.x;
    auto tileY = tile.y + position.y;

    if(tileX < 0 || tileX > 9 || tileY < 0 || tileY > 24)
    {
      success = false;
      break;
    }
    else if(mFallenTiles[tileY][tileX] == 1)
    {
      success = false;
      break;
    }
  }

  return success;
}

/******************************************************************************/
void BlockSystem::RemoveFilledRows()
{
  // First, determine which rows, if any, are filled.
  std::vector<int> filledRows;
  for(int rowIndex = 0; rowIndex < 25; ++rowIndex)
  {
    bool rowFilled = true;
    for(const auto& tile : mFallenTiles[rowIndex])
    {
      if(tile == 0)
      {
        rowFilled = false;
        break;
      }
    }

    if(rowFilled)
    {
      filledRows.emplace_back(rowIndex);
    }
  }

  // For each filled row, add each Entity in the row into the list of
  // Entities to remove.
  for(const auto& rowIndex : filledRows)
  {
    for(int column = 0; column < 10; ++column)
    {
      mEntitiesToRemove.emplace_back(mFallenTileEntities[rowIndex][column]);
    }
  }

  // For each filled row, copy the values of the above row into it. Then repeat
  // the process all the way up to the top of the grid.
  int clearedRows = 0;
  for(const auto& rowIndex : filledRows)
  {
    for(int row = rowIndex - clearedRows; row < 25; ++row)
    {
      for(int column = 0; column < 10; ++column)
      {
        // If this row is the top of the grid, fill the row with zeroes.
        // Otherwise, copy the values of the row above it.
        if(row == 24)
        {
          mFallenTiles[row][column] = 0;
        }
        else
        {
          mFallenTiles[row][column] = mFallenTiles[row + 1][column];
          mFallenTileEntities[row][column] = mFallenTileEntities[row + 1][column];
        }
      }
    }

    ++clearedRows;
  }
}

/******************************************************************************/
void BlockSystem::UpdateFallenTileEntities(Kuma3D::Scene& aScene)
{
  for(int row = 0; row < 25; ++row)
  {
    for(int column = 0; column < 10; ++column)
    {
      if(mFallenTiles[row][column])
      {
        auto& entity = mFallenTileEntities[row][column];
        auto& movable = aScene.GetComponentForEntity<Movable>(entity);

        movable.mTargetPosition.x = (column * mTileSizeInPixels) + (mTileSizeInPixels / 2.0);
        movable.mTargetPosition.y = (row * mTileSizeInPixels) + (mTileSizeInPixels / 2.0);
        movable.mMoveSpeed = 0.5;
        movable.mFinishedMoving = false;
      }
    }
  }
}

} // namespace KumaTetris
