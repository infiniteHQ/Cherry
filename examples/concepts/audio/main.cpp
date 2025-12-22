#include "../../../cherry.hpp"

void Render() {
  static bool initialized = false;
  if (!initialized) {
    CherryApp.CreateChannel("SFX");
    CherryApp.GetChannel("SFX")->AddSound(
        CherryPath("resources/audio/tick.wav"));
    CherryApp.GetChannel("SFX")->AddSound(
        CherryPath("resources/audio/tick2.wav"));
    CherryApp.CreateChannel("Music");
    CherryApp.GetChannel("Music")->AddSound(
        CherryPath("resources/audio/tick.wav"));
    CherryApp.GetChannel("Music")->AddSound(
        CherryPath("resources/audio/tick2.wav"));
    initialized = true;
  }

  for (auto const &[name, channel] : CherryApp.GetChannels()) {
    channel->Update();

    if (ImGui::CollapsingHeader(name.c_str())) {
      ImGui::PushID(name.c_str());

      if (ImGui::Button("Play One (Tick)")) {
        channel->PlayTick();
      }

      ImGui::SameLine();

      if (ImGui::Button("Start Loop")) {
        channel->Play();
      }

      ImGui::SameLine();

      if (ImGui::Button("Stop")) {
        channel->Stop();
      }

      ImGui::Separator();

      ImGui::SliderFloat("Volume", &channel->m_settings.volume, 0.0f, 2.0f);
      ImGui::SliderFloat("Pan", &channel->m_settings.pan, -1.0f, 1.0f);
      ImGui::Checkbox("Randomize Next", &channel->m_settings.play_random);

      ImGui::PopID();
    }
  }
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.UseAudio();
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}