#include "EntitySignals.hpp"

namespace Kuma3D {

SignalT<Entity, const Signature&> EntitySignatureChanged;
SignalT<Entity> EntityPendingDeletion;

} // namespace Kuma3D
