#ifndef MOVEMENTSYSTEM_HPP
#define MOVEMENTSYSTEM_HPP

#include <System.hpp>
#include <Types.hpp>

namespace example_2
{
  class MovementSystem : public Kuma3D::System
  {
    public:
      MovementSystem();

      /**
       * Initializes the System by registering the Movable and Transform
       * components.
       *
       * @param aScene The Scene this System was added to.
       */
      void Initialize(Kuma3D::Scene& aScene) override;

      /**
       * Moves each Entity with a Movable component and a Transform component.
       *
       * @param aScene The Scene containing the Entities' component data.
       * @param aTime The start time of the current frame.
       */
      void Operate(Kuma3D::Scene& aScene, double aTime) override;

    private:
      double mPrevTime;
  };
}

#endif
