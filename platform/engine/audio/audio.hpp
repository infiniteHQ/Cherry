#ifndef CHERRY_AUDIO_HPP
#define CHERRY_AUDIO_HPP

#ifdef CHERRY_ENABLE_AUDIO

#include "../../../lib/miniaudio/miniaudio.h"
#include <algorithm>
#include <atomic>
#include <map>
#include <random>
#include <string>
#include <vector>

struct ChannelSettings {
  float volume = 1.0f;
  float pan = 0.0f;
  bool play_random = false;
};

class AudioChannel {
public:
  AudioChannel(ma_engine *engine, const std::string &name);
  ~AudioChannel();

  void AddSound(const std::string &path);
  void PlayTick();
  void Play();
  void Stop();
  void Update();
  void ApplySettings();

  ChannelSettings m_settings;
  std::string m_name;

private:
  void StopCurrentSound();
  int CalculateNextIndex();
  void InternalPlayNextWithCallback();

  ma_engine *m_engine;
  std::vector<std::string> m_playlist;
  ma_sound m_currentSound;
  int m_currentIndex = 0;
  bool m_isPlaying = false;
  std::atomic<bool> m_isLoopingActive{false};
  std::atomic<bool> m_needsRestart{false};
};

#endif // CHERRY_ENABLE_AUDIO
#endif // CHERRY_AUDIO_HPP