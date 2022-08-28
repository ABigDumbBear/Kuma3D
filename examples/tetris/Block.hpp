#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <Mat4.hpp>

namespace Tetris {

struct GridPosition
{
  int x { 0 };
  int y { 0 };
};

struct Block
{
  GridPosition mTiles[4];
};

const Kuma3D::Mat4 L1 =
{
  0, 0, 0, 0,
  0, 0, 0, 1,
  1, 1, 1, 0,
  0, 0, 0, 0
};

} // namespace Tetris

#endif
