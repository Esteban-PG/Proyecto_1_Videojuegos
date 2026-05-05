#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <SDL2/SDL_mixer.h>

#include <map>
#include <string>

/**
 * @brief Manages all game audio via SDL_mixer.
 *
 * Wraps SDL_mixer to load and play music tracks and sound effect chunks.
 * All methods silently log and return on failure so the game continues
 * without audio if files are missing.
 *
 * Sound effect IDs expected by Nitro Rush:
 *   - "engine"   : looping car engine hum
 *   - "nitro"    : nitro activation burst
 *   - "jump"     : ramp jump whoosh
 *   - "bounce"   : wall bounce thud
 *   - "death"    : player death
 *   - "win"      : reaching the checkpoint
 *
 * Music track IDs expected:
 *   - "music_menu"   : main menu background
 *   - "music_level1" : city/asphalt theme
 *   - "music_level2" : mountain/dirt theme
 *   - "music_level3" : factory/night theme
 */
class AudioManager {
 private:
  std::map<std::string, Mix_Music*> musicTracks; ///< Loaded BGM tracks
  std::map<std::string, Mix_Chunk*> sfxChunks;   ///< Loaded sound effects

 public:
  AudioManager();
  ~AudioManager();

  /**
   * @brief Initialises SDL_mixer (44.1 kHz, stereo, 2 KB buffer).
   * @return true on success
   */
  bool init();

  /** @brief Release all loaded audio assets and close SDL_mixer. */
  void destroy();

  /**
   * @brief Load a music file (OGG / MP3 / WAV).
   * @param id   Key used to reference this track later
   * @param path File path relative to the executable
   */
  void addMusic(const std::string& id, const std::string& path);

  /**
   * @brief Load a sound-effect file (WAV / OGG).
   * @param id   Key used to reference this chunk later
   * @param path File path relative to the executable
   */
  void addSFX(const std::string& id, const std::string& path);

  /**
   * @brief Start (or restart) a music track.
   * @param id    Track key loaded with addMusic()
   * @param loops Number of additional repeats (-1 = infinite)
   */
  void playMusic(const std::string& id, int loops = -1);

  /** @brief Fade the current music out over the given milliseconds. */
  void stopMusic(int fadeOutMs = 300);

  /**
   * @brief Play a one-shot sound effect.
   * @param id      Chunk key loaded with addSFX()
   * @param channel Mixer channel (-1 = first available)
   * @param loops   Additional repeats (0 = play once)
   */
  void playSFX(const std::string& id, int channel = -1, int loops = 0);

  /** @brief True if the mixer was successfully initialised. */
  bool isReady() const { return ready; }

 private:
  bool ready = false; ///< Set to true after successful init()
};

#endif  // AUDIO_MANAGER_HPP
