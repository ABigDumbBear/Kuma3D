#ifndef MOVABLE_HPP
#define MOVABLE_HPP

#include <Vec3.hpp>

namespace example_2
{
  struct Movable
  {
    Kuma3D::Vec3 mDirection;

    double mSpeed { 0.02 };
  };
}

#endif
