#include "audio_manager.hpp"

#include <iostream>

AudioManager::AudioManager() {
  std::cout << "[AUDIO] AudioManager created." << std::endl;
}

AudioManager::~AudioManager() {
  destroy();
}

bool AudioManager::init() {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "[AUDIO] SDL_mixer init failed: " << Mix_GetError() << std::endl;
    return false;
  }
  ready = true;
  std::cout << "[AUDIO] SDL_mixer initialised." << std::endl;
  return true;
}

void AudioManager::destroy() {
  for (auto& [id, chunk] : sfxChunks) {
    if (chunk) Mix_FreeChunk(chunk);
  }
  sfxChunks.clear();

  for (auto& [id, music] : musicTracks) {
    if (music) Mix_FreeMusic(music);
  }
  musicTracks.clear();

  if (ready) {
    Mix_CloseAudio();
    ready = false;
  }
}

void AudioManager::addMusic(const std::string& id, const std::string& path) {
  if (!ready) return;
  Mix_Music* music = Mix_LoadMUS(path.c_str());
  if (!music) {
    std::cerr << "[AUDIO] Failed to load music '" << id << "': "
              << Mix_GetError() << std::endl;
    return;
  }
  musicTracks[id] = music;
  std::cout << "[AUDIO] Loaded music: " << id << std::endl;
}

void AudioManager::addSFX(const std::string& id, const std::string& path) {
  if (!ready) return;
  Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
  if (!chunk) {
    std::cerr << "[AUDIO] Failed to load SFX '" << id << "': "
              << Mix_GetError() << std::endl;
    return;
  }
  sfxChunks[id] = chunk;
  std::cout << "[AUDIO] Loaded SFX: " << id << std::endl;
}

void AudioManager::playMusic(const std::string& id, int loops) {
  if (!ready) return;
  auto it = musicTracks.find(id);
  if (it == musicTracks.end()) {
    std::cerr << "[AUDIO] Music not found: " << id << std::endl;
    return;
  }
  Mix_PlayMusic(it->second, loops);
}

void AudioManager::stopMusic(int fadeOutMs) {
  if (!ready) return;
  Mix_FadeOutMusic(fadeOutMs);
}

void AudioManager::playSFX(const std::string& id, int channel, int loops) {
  if (!ready) return;
  auto it = sfxChunks.find(id);
  if (it == sfxChunks.end()) return;
  Mix_PlayChannel(channel, it->second, loops);
}
