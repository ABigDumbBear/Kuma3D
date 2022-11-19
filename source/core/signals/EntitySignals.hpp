#ifndef ENTITYSIGNALS_HPP
#define ENTITYSIGNALS_HPP

#include "Signal.hpp"
#include "Types.hpp"

namespace Kuma3D {

extern SignalT<Entity, const Signature&> EntitySignatureChanged;
extern SignalT<Entity> EntityPendingDeletion;

} // namespace Kuma3D

#endif
