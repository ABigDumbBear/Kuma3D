#ifndef ROTATESYSTEM_HPP
#define ROTATESYSTEM_HPP

#include <System.hpp>

#include <vector>

#include <InputSignals.hpp>
#include <Observer.hpp>

namespace Lighting {

class RotateSystem : public Kuma3D::System
{
  public:
    void Initialize(Kuma3D::Scene& aScene) override;
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:
    void HandleKeyPressed(const Kuma3D::KeyCode& aKey, int aMods);
    void HandleKeyReleased(const Kuma3D::KeyCode& aKey, int aMods);

    double mTime { 0 };

    std::vector<Kuma3D::KeyCode> mPressedKeys;

    Kuma3D::Observer mObserver;
};

} // namespace Lighting

#endif
