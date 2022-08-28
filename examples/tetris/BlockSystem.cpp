#include "BlockSystem.hpp"

#include <Scene.hpp>

#include "Block.hpp"

namespace Tetris {

const float BlockSystem::mFallSpeed = 0.5;

/******************************************************************************/
void BlockSystem::Initialize(Kuma3D::Scene& aScene)
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
void BlockSystem::Operate(Kuma3D::Scene& aScene, double aTime)
{
  auto dt = aTime - mTimeSinceLastFall;
  if(dt >= mFallSpeed)
  {
    for(const auto& entity : GetEntities())
    {
      auto& block = aScene.GetComponentForEntity<Block>(entity);

      --block.mPosition.y;
      for(auto& tile : block.mTiles)
      {
        --tile.y;
      }
    }

    mTimeSinceLastFall = aTime;
  }
}

} // namespace Tetris
