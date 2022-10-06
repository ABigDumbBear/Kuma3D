#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <System.hpp>

namespace StarBear {

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
     * Checks each Entity with a Hitbox component to see if a collision has
     * occurred.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  protected:

    /**
     * A handler function that gets called whenever an Entity is added
     * to this System.
     *
     * @param aEntity The Entity that was added.
     */
    void HandleEntityBecameEligible(const Kuma3D::Entity& aEntity) override;

    /**
     * A handler function that gets called whenever an Entity is removed
     * from this System.
     *
     * @param aEntity The Entity that was removed.
     */
    void HandleEntityBecameIneligible(const Kuma3D::Entity& aEntity) override;

  private:
    std::vector<Kuma3D::Entity> mEntityGrid[10][10][50];
    std::vector<Kuma3D::Entity> mNewEntities;

    static const int mGridCellWidth { 1 };
    static const int mGridCellHeight { 1 };
    static const int mGridCellLength { 1 };
};

} // namespace StarBear

#endif
