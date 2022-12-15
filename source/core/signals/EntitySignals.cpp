#include "EntitySignals.hpp"

namespace Kuma3D {

SignalT<Entity, const Signature&> EntitySignatureChanged;
SignalT<Entity, Scene&> EntityRemoved;
SignalT<Entity, const Scene&> EntityPendingDeletion;

} // namespace Kuma3D
