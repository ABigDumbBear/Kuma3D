#ifndef SHIPCONTROL_HPP
#define SHIPCONTROL_HPP

namespace StarBear {

struct ShipControl
{
  double mMaxSpeed { 50.0 };
  double mCurrentSpeed { 0.0 };

  double mAcceleration { 250.0 };

  double mMaxRotation { 35.0 };
};

} // namespace StarBear

#endif
