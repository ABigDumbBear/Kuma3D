#ifndef GAMESIGNALS_HPP
#define GAMESIGNALS_HPP

#include <Signal.hpp>

namespace Kuma3D {

extern SignalT<int, int> FramebufferResized;
extern SignalT<double> GamePendingExit;

} // namespace Kuma3D

#endif
