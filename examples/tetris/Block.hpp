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

const Block LBlock
{
  {0, 0},
  {
    {0, 0},
    {1, 0},
    {2, 0},
    {2, 1}
  }
};

const Block IBlock
{
  {0, 0},
  {
    {0, 0},
    {1, 0},
    {2, 0},
    {3, 0}
  }
};

const Block SBlock
{
  {0, 0},
  {
    {0, 0},
    {1, 0},
    {1, 1},
    {2, 1}
  }
};

} // namespace Tetris

#endif
