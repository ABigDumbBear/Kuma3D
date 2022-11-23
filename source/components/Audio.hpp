#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "IDGenerator.hpp"

namespace Kuma3D {

struct Audio
{
  ID mSoundID { 0 };

  float mVolume { 1.0 };
  bool mLooping { false };
};

} // namespace Kuma3D

#endif
