#include "CollisionSystem.hpp"

#include <Scene.hpp>

#include "Block.hpp"

namespace Tetris {

/******************************************************************************/
void CollisionSystem::Initialize(Kuma3D::Scene& aScene)
{
  if(!aScene.IsComponentTypeRegistered<Block>())
  {
    aScene.RegisterComponentType<Block>();
  }

  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Block>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void CollisionSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  for(const auto& entity : GetEntities())
  {
    auto& block = aScene.GetComponentForEntity<Block>(entity);

    // For each tile in the block, check the list of fallen tiles. If one of
    // the fallen tiles is directly underneath the tile in the block, the block
    // needs to stop moving.
    bool blockFallen = false;
    for(const auto& blockTile : block.mTiles)
    {
      for(const auto& fallenTile : mFallenTiles)
      {
        if(fallenTile.x == blockTile.x &&
           fallenTile.y == blockTile.y - 1)
        {
          blockFallen = true;
          break;
        }
      }

      if(blockFallen)
      {
        break;
      }
    }

    // If the block has fallen, take each of the tiles in the block and add
    // them to the list of fallen tiles.
    if(blockFallen)
    {
      for(const auto& blockTile : block.mTiles)
      {
        mFallenTiles.emplace_back(blockTile);
      }
    }
  }
}

} // namespace Tetris
