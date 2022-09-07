#ifndef SHIPCONTROLSYSTEM_HPP
#define SHIPCONTROLSYSTEM_HPP

#include <System.hpp>

#include <InputSignals.hpp>
#include <Observer.hpp>

namespace StarBear {

class ShipControlSystem : public Kuma3D::System
{
  public:

    /**
     * Initializes the System by registering the relevant components.
     *
     * @param aScene The Scene this System was added to.
     */
    void Initialize(Kuma3D::Scene& aScene) override;

    /**
     * Moves each Entity with a ShipControl and a Transform component.
     *
     * @param aScene The Scene containing the Entities' component data.
     * @param aTime The start time of the current frame.
     */
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:

    /**
     * A handler function that gets called whenever a key is pressed.
     *
     * @param aKey The key that was pressed.
     * @param aMods Any mods active on the key when it was pressed.
     */
    void HandleKeyPressed(const Kuma3D::KeyCode& aKey, int aMods);

    /**
     * A handler function that gets called whenever a key is released.
     *
     * @param aKey The key that was released.
     * @param aMods Any mods active on the key when it was pressed.
     */
    void HandleKeyReleased(const Kuma3D::KeyCode& aKey, int aMods);

    std::vector<Kuma3D::KeyCode> mPressedKeys;

    double mTimeSinceLastUpdate { 0.0 };

    Kuma3D::Observer mObserver;
};

} // namespace StarBear

#endif
