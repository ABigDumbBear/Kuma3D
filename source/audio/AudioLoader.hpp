#ifndef AUDIOLOADER_HPP
#define AUDIOLOADER_HPP

#include <map>
#include <memory>
#include <string>

#include "IDGenerator.hpp"

class ma_engine;
class ma_sound;

class AudioSystem;

namespace Kuma3D {

/**
 * A static class that handles loading audio files.
 */
class AudioLoader
{
  // Only the AudioSystem is able to play sounds.
  friend class AudioSystem;

  public:

    /**
     * Loads the audio data from a given file.
     *
     * @param aFilePath The path to an audio file.
     * @return The ID of the stored audio data.
     */
    static ID LoadAudioFromFile(const std::string& aFilePath);

    /**
     * Unloads all audio data and uninitializes the audio library. If you load
     * any audio, be sure to call this before the program terminates to avoid
     * memory leaks.
     */
    static void UnloadAudio();

  private:

    /**
     * Returns the ma_sound object associated with the given ID.
     *
     * @param aID The ID of the sound object.
     * @return The ma_sound object with the given ID.
     */
    static ma_sound& GetSound(const ID& aID);

    /**
     * Removes the ma_sound object associated with the given ID.
     *
     * @param aID The ID of the sound object.
     */
    static void RemoveSound(const ID& aID);

    static ma_engine mAudioEngine;
    static std::map<ID, ma_sound> mAudioMap;

    static IDGenerator mIDGenerator;
    static bool mInitialized;
};

} // namespace Kuma3D

#endif
