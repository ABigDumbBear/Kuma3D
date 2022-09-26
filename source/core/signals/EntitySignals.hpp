#ifndef ENTITYSIGNALS_HPP
#define ENTITYSIGNALS_HPP

#include "Signal.hpp"
#include "Types.hpp"

namespace Kuma3D {

extern SignalT<const Entity&, const Signature&> EntitySignatureChanged;
extern SignalT<const Entity&> EntityPendingDeletion;

} // namespace Kuma3D

#endif
