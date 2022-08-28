#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <System.hpp>

#include <vector>

#include "Block.hpp"

namespace Tetris {

class CollisionSystem : public Kuma3D::System
{
  public:

    /**
     * Initializes the System by registering the relevant components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Kuma3D::Scene& aScene) override;

    /**
     * Checks each Entity with a Block component to see if it has collided with
     * any fallen tiles or if it has reached the bottom of the grid.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:
    std::vector<GridPosition> mFallenTiles;
};

} // namespace Tetris

#endif
