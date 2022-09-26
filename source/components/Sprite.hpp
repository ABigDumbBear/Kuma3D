#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <map>
#include <string>
#include <vector>

#include "Types.hpp"

namespace Kuma3D {

/**
 * A struct that defines a portion of a 2D texture. All values are expected
 * to be in pixels.
 */
struct TextureClip
{
  unsigned int mLeft { 0 };
  unsigned int mRight { 0 };
  unsigned int mBottom { 0 };
  unsigned int mTop { 0 };
};

struct Animation
{
  std::vector<TextureClip> mFrames;
  unsigned int mCurrentFrame { 0 };
  bool mLoop { true };
};

struct Sprite
{
  ID mSpritesheetTextureID { 0 };

  std::map<std::string, Animation> mAnimations;
  std::string mCurrentAnimation;
  float mAnimationSpeed { 1 };

  bool mFlipX { false };
  bool mFlipY { false };

  bool mDirty { false };
};

} // namespace Kuma3D

#endif
