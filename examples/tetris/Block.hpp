#ifndef BLOCK_HPP
#define BLOCK_HPP

namespace Tetris {

struct GridPosition
{
  int x { 0 };
  int y { 0 };
};

struct Block
{
  GridPosition mPosition;
  GridPosition mTiles[4];
};

} // namespace Tetris

#endif
