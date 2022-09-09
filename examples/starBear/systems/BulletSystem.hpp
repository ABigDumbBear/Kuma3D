#ifndef BULLETSYSTEM_HPP
#define BULLETSYSTEM_HPP

#include <System.hpp>

namespace StarBear {

class BulletSystem : public Kuma3D::System
{
  public:

    /**
     * Initializes the System by registering the relevant components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Kuma3D::Scene& aScene) override;

    /**
     * Moves each Entity with a Bullet and a Transform component.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:
    double mTimeSinceLastUpdate { 0.0 };
};

} // namespace StarBear

#endif
