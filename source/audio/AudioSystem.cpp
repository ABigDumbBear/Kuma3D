#include "AudioSystem.hpp"

#include "miniaudio.h"

#include "Scene.hpp"

#include "Audio.hpp"
#include "AudioLoader.hpp"

namespace Kuma3D {

/******************************************************************************/
void AudioSystem::Initialize(Scene& aScene)
{
  // Register the Audio component.
  if(!aScene.IsComponentTypeRegistered<Audio>())
  {
    aScene.RegisterComponentType<Audio>();
  }

  // Set the signature to care about entities with Audio components.
  auto signature = aScene.CreateSignature();
  signature[aScene.GetComponentIndex<Audio>()] = true;
  SetSignature(signature);
}

/******************************************************************************/
void AudioSystem::Operate(Scene& aScene, double aTime)
{
  for(const auto& entity : GetEntities())
  {
    auto& audio = aScene.GetComponentForEntity<Audio>(entity);
    auto& sound = AudioLoader::GetSound(audio.mSoundID);

    ma_sound_set_volume(&sound, audio.mVolume);
    ma_sound_set_looping(&sound, audio.mLooping);

    // If the sound isn't playing already, play it.
    if(!ma_sound_is_playing(&sound))
    {
      ma_sound_start(&sound);
    }
  }
}

} // namespace Kuma3D
