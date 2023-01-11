#ifndef POINTLIGHTSYSTEM_HPP
#define POINTLIGHTSYSTEM_HPP

#include "System.hpp"

namespace Kuma3D {

class PointLightSystem : public System
{
  public:
    void Initialize(Scene& aScene) override;
    void Operate(Scene& aScene, double aTime) override;
};

} // namespace Kuma3D

#endif
