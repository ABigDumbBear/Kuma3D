#include "Observer.hpp"

#include <algorithm>

#include "Signal.hpp"

namespace Kuma3D {

/******************************************************************************/
Observer::~Observer()
{
  for(auto& s : mConnectedSignals)
  {
    s->Disconnect(*this);
  }
}

/******************************************************************************/
bool Observer::Add(Signal& aSignal)
{
  bool success = true;

  auto s = std::find(mConnectedSignals.begin(),
                     mConnectedSignals.end(),
                     &aSignal);
  if(s == mConnectedSignals.end())
  {
    mConnectedSignals.emplace_back(&aSignal);
  }
  else
  {
    success = false;
  }

  return success;
}

} // namespace Kuma3D
