#include "AudioLoader.hpp"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <stdexcept>
#include <sstream>

#include <iostream>

namespace Kuma3D {

/******************************************************************************/
ma_engine AudioLoader::mAudioEngine;
std::map<std::string, ID> AudioLoader::mAudioFileMap;
std::map<ID, ma_sound> AudioLoader::mAudioMap;

IDGenerator AudioLoader::mIDGenerator;
bool AudioLoader::mInitialized = false;

/******************************************************************************/
ID AudioLoader::LoadAudioFromFile(const std::string& aFilePath)
{
  ID audioID = 0;

  // Initialize the Miniaudio library if it isn't already.
  if(!mInitialized)
  {
    auto errorCode = ma_engine_init(nullptr, &mAudioEngine);
    if(errorCode != MA_SUCCESS)
    {
      std::stringstream error;
      error << "Failed to initialize audio library! Miniaudio error code: " << errorCode;
      throw std::runtime_error(error.str());
    }

    mInitialized = true;
  }

  // Don't load the same audio file twice.
  auto foundID = mAudioFileMap.find(aFilePath);
  if(foundID != mAudioFileMap.end())
  {
    audioID = foundID->second;
  }
  else
  {
    // Generate a new ID and a new sound object.
    audioID = mIDGenerator.GenerateID();
    mAudioMap.emplace(audioID, ma_sound());

    // Attempt to load the audio file.
    auto errorCode = ma_sound_init_from_file(&mAudioEngine,
                                             aFilePath.c_str(),
                                             0,
                                             nullptr,
                                             nullptr,
                                             &mAudioMap[audioID]);
    if(errorCode != MA_SUCCESS)
    {
      mIDGenerator.RemoveID(audioID);
      mAudioMap.erase(audioID);

      std::stringstream error;
      error << "Failed to load audio file! Miniaudio error code: " << errorCode;
      throw std::runtime_error(error.str());
    }

    mAudioFileMap.emplace(aFilePath, audioID);
  }

  return audioID;
}

/******************************************************************************/
void AudioLoader::UnloadAudio()
{
  // Uninitialize all loaded sounds.
  for(auto& soundPair : mAudioMap)
  {
    mIDGenerator.RemoveID(soundPair.first);
    ma_sound_uninit(&soundPair.second);
  }
  mAudioMap.clear();

  // Uninitialize the audio engine.
  ma_engine_uninit(&mAudioEngine);
  mInitialized = false;
}

/******************************************************************************/
ma_sound& AudioLoader::GetSound(const ID& aID)
{
  auto foundSound = mAudioMap.find(aID);
  if(foundSound == mAudioMap.end())
  {
    std::stringstream error;
    error << "No sound with ID " << aID << " exists!";
    throw std::invalid_argument(error.str());
  }

  return foundSound->second;
}

} // namespace Kuma3D
