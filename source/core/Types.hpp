#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>

namespace Kuma3D {

// In the Kuma3D, a game entity is just a unique integer.
using ID = unsigned int;
using Entity = ID;

// A Signature is used to identify what components are attached to an Entity.
using Signature = std::vector<bool>;

} // namespace Kuma3D

#endif
