#ifndef AUDIOSYSTEM_HPP
#define AUDIOSYSTEM_HPP

#include "System.hpp"

namespace Kuma3D {

class AudioSystem : public System
{
  public:

    /**
     * Initializes the system by registering the Audio component.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Scene& aScene) override;

    /**
     * Plays the sound associated with each Audio component.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Scene& aScene, double aTime) override;

  private:
    std::vector<Entity> mCurrentlyPlayingEntities;
};

} // namespace Kuma3D

#endif
