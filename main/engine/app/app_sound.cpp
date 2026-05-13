//
//  app_sound.cpp
//  Sources for main sound engine feature
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "app.hpp"

namespace Cherry {

#ifdef CHERRY_ENABLE_AUDIO
  void Application::StartAudioService() {
    ma_result result;

    result = ma_engine_init(NULL, &this->m_AudioEngine);
    if (result != MA_SUCCESS) {
      std::cout << "Error: Failed to start audio service" << std::endl;
    }
  }

  void Application::StopAudioService() {
    ma_engine_uninit(&this->m_AudioEngine);
  }

  void Application::PlaySound(const std::string &wav_file_path) {
    ma_engine_play_sound(&this->m_AudioEngine, wav_file_path.c_str(), NULL);
  }

  void Application::CreateChannel(const std::string &name) {
    m_Channels[name] = std::make_shared<AudioChannel>(&m_AudioEngine, name);
  }

  std::shared_ptr<AudioChannel> Application::GetChannel(const std::string &name) {
    if (m_Channels.find(name) != m_Channels.end())
      return m_Channels[name];
    return nullptr;
  }

  std::map<std::string, std::shared_ptr<AudioChannel>> &Application::GetChannels() {
    return m_Channels;
  }
#else
  void Application::StartAudioService() {
    //
  }

  void Application::StopAudioService() {
    //
  }

  void Application::PlaySound(const std::string &wav_file_path) {
    std::cout << "To use audio, please enable CHERRY_ENABLE_AUDIO (to 1) on "
                 "options.hpp of the Cherry framework"
              << std::endl;
  }
#endif

}  // namespace Cherry