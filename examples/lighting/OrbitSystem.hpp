#ifndef ORBITSYSTEM_HPP
#define ORBITSYSTEM_HPP

#include <System.hpp>

namespace Lighting {

class OrbitSystem : public Kuma3D::System
{
  public:
    void Initialize(Kuma3D::Scene& aScene) override;
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:
    double mTime { 0 };
};

} // namespace Lighting

#endif
