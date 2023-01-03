#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include <System.hpp>

namespace Cubes {

class PhysicsSystem : public Kuma3D::System
{
  public:
    void Initialize(Kuma3D::Scene& aScene) override;
    void Operate(Kuma3D::Scene& aScene, double aTime) override;

  private:
    double mTime { 0 };
};

} // namespace Cubes

#endif
