#include "EntitySignals.hpp"

namespace Kuma3D {

SignalT<const Entity&, const Signature&> EntitySignatureChanged;
SignalT<const Entity&> EntityPendingDeletion;

} // namespace Kuma3D
