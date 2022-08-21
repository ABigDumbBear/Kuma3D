#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>

namespace Kuma3D {

class Signal;

/**
 * All Observers automatically disconnect themselves from any
 * Signals when destroyed.
 */
class Observer
{
  // Only Signals can add themselves to the Observer.
  template<class ...Args> friend class SignalT;

  public:

    /**
     * Destructor. When destroyed, the Observer class disconnects itself
     * from each connected Signal; this way, when the Signal is
     * next notified, it won't be calling dead functions.
     */
    ~Observer();

  protected:

    /**
     * Adds a Signal to keep track of, if it isn't already being tracked.
     *
     * @param aSignal The Signal to keep track of.
     * @return True if successful, false otherwise.
     */
    bool Add(Signal& aSignal);

  private:
    std::vector<Signal*> mConnectedSignals;
};

} // namespace Kuma3D

#endif
