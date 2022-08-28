#ifndef BLOCKSYSTEM_HPP
#define BLOCKSYSTEM_HPP

#include <System.hpp>

namespace Tetris {

class BlockSystem : public Kuma3D::System
{
  public:

    /**
     * Initializes the System by registering the relevant components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Kuma3D::Scene& aScene) override;

    /**
     * Moves each Entity with a Block component.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:
    float mTimeSinceLastFall { 0.0 };

    static const float mFallSpeed;
};

} // namespace Tetris

#endif
