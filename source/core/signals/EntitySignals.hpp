#ifndef ENTITYSIGNALS_HPP
#define ENTITYSIGNALS_HPP

#include "Signal.hpp"

#include "Entity.hpp"
#include "Scene.hpp"
#include "Signature.hpp"

namespace Kuma3D {

extern SignalT<Entity, const Signature&> EntitySignatureChanged;
extern SignalT<Entity, const Scene&> EntityPendingDeletion;

} // namespace Kuma3D

#endif
