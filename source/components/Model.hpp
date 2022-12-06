#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include "Entity.hpp"

namespace Kuma3D {

struct Model
{
  std::vector<Entity> mChildren;
};

} // namespace Kuma3D

#endif
