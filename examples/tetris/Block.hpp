#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <Vec3.hpp>

namespace Tetris {

struct GridPosition
{
  int x { 0 };
  int y { 0 };
};

struct Block
{
  GridPosition mPosition;

  GridPosition mRotations[4][4];
  int mCurrentRotation { 0 };

  Kuma3D::Vec3 mColor;
};

const Block IBlock
{
  // Initial position
  {0, 0},

  // Rotations
  {
    // Rotation 0
    {
      {0, 0}, {1, 0}, {2, 0}, {3, 0}
    },

    // Rotation 1
    {
      {0, 0}, {0, 1}, {0, 2}, {0, 3}
    },

    // Rotation 2
    {
      {0, 0}, {1, 0}, {2, 0}, {3, 0}
    },

    // Rotation 3
    {
      {0, 0}, {0, 1}, {0, 2}, {0, 3}
    },
  },

  // Initial rotation
  0,

  // Color
  {1.0, 0.0, 0.0}
};

const Block LBlock
{
  // Initial position
  {0, 0},

  // Rotations
  {
    // Rotation 0
    {
      {0, 0}, {1, 0}, {2, 0}, {2, 1}
    },

    // Rotation 1
    {
      {0, 2}, {0, 1}, {0, 0}, {1, 0}
    },

    // Rotation 2
    {
      {0, 0}, {0, 1}, {1, 1}, {2, 1}
    },

    // Rotation 3
    {
      {0, 2}, {1, 2}, {1, 1}, {1, 0}
    },
  },

  // Initial rotation
  0,

  // Color
  {0.0, 1.0, 0.0}
};

const Block SBlock
{
  // Initial position
  {0, 0},

  // Rotations
  {
    // Rotation 0
    {
      {0, 0}, {1, 0}, {1, 1}, {2, 1}
    },

    // Rotation 1
    {
      {0, 2}, {0, 1}, {1, 1}, {1, 0}
    },

    // Rotation 2
    {
      {0, 1}, {1, 1}, {1, 0}, {2, 0}
    },

    // Rotation 3
    {
      {0, 0}, {0, 1}, {1, 1}, {1, 2}
    },
  },

  // Initial rotation
  0,

  // Color
  {0.0, 0.0, 1.0}
};

} // namespace Tetris

#endif
