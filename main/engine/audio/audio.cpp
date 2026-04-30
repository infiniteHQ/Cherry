#include "audio.hpp"

#ifdef CHERRY_ENABLE_AUDIO

AudioChannel::AudioChannel(ma_engine *engine, const std::string &name)
    : m_engine(engine), m_name(name) {}

AudioChannel::~AudioChannel() { Stop(); }

void AudioChannel::AddSound(const std::string &path) {
  m_playlist.push_back(path);
}

void AudioChannel::PlayTick() {
  if (m_playlist.empty())
    return;

  StopCurrentSound();

  int index = CalculateNextIndex();
  ma_result result = ma_sound_init_from_file(
      m_engine, m_playlist[index].c_str(), 0, NULL, NULL, &m_currentSound);

  if (result == MA_SUCCESS) {
    ApplySettings();
    ma_sound_start(&m_currentSound);
    m_isPlaying = true;
    m_currentIndex = (index + 1) % m_playlist.size();
  }
}

void AudioChannel::Play() {
  Stop();
  m_isLoopingActive = true;
  InternalPlayNextWithCallback();
}

void AudioChannel::Stop() {
  m_isLoopingActive = false;
  StopCurrentSound();
}

void AudioChannel::Update() {
  if (m_needsRestart) {
    m_needsRestart = false;
    StopCurrentSound();
    InternalPlayNextWithCallback();
  }
  ApplySettings();
}

void AudioChannel::ApplySettings() {
  if (m_isPlaying) {
    ma_sound_set_volume(&m_currentSound, m_settings.volume);
    ma_sound_set_pan(&m_currentSound, m_settings.pan);
  }
}

void AudioChannel::StopCurrentSound() {
  if (m_isPlaying) {
    ma_sound_stop(&m_currentSound);
    ma_sound_uninit(&m_currentSound);
    m_isPlaying = false;
  }
}

int AudioChannel::CalculateNextIndex() {
  if (m_settings.play_random && m_playlist.size() > 1) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, (int)m_playlist.size() - 1);
    return dist(gen);
  }
  return m_currentIndex % m_playlist.size();
}

void AudioChannel::InternalPlayNextWithCallback() {
  if (m_playlist.empty())
    return;

  int index = CalculateNextIndex();
  ma_result result = ma_sound_init_from_file(
      m_engine, m_playlist[index].c_str(), 0, NULL, NULL, &m_currentSound);

  if (result == MA_SUCCESS) {
    ApplySettings();

    ma_sound_set_end_callback(
        &m_currentSound,
        [](void *pUserData, ma_sound *pSound) {
          auto *channel = static_cast<AudioChannel *>(pUserData);
          if (channel->m_isLoopingActive) {
            channel->m_needsRestart = true;
          }
        },
        this);

    ma_sound_start(&m_currentSound);
    m_isPlaying = true;
    m_currentIndex = (index + 1) % m_playlist.size();
  }
}

#endif // CHERRY_ENABLE_AUDIO