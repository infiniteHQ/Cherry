#include "../../../cherry.hpp"
#include <cmath>
#include <vector>

namespace Cherry {
namespace Components {

struct Particle {
  float angle;
  float radius;
  float speed;
};

class OrbitalVisualizer : public Component {
private:
  std::vector<Particle> m_Particles;
  const int m_Count = 100;

public:
  OrbitalVisualizer(const Cherry::Identifier &id) : Component(id) {
    SetIdentifier(id);

    SetProperty("speed_mult", 1.0f);
    SetProperty("amplitude", 50.0f);
    SetProperty("thickness", 2.0f);
    SetProperty("color", "#00F2FF");

    for (int i = 0; i < m_Count; i++) {
      m_Particles.push_back({(float)i * (6.28f / m_Count), 20.0f + (i * 0.5f),
                             0.5f + ((float)rand() / RAND_MAX)});
    }
  }

  void Render() override {
    ImDrawList *draw_list = CherryGUI::GetWindowDrawList();
    ImVec2 center = CherryGUI::GetCursorScreenPos() + ImVec2(150, 150);

    float speed_mult = GetPropertyAsFloat("speed_mult");
    float amplitude = GetPropertyAsFloat("amplitude");
    float thickness = GetPropertyAsFloat("thickness");
    ImU32 color =
        CherryGUI::ColorConvertFloat4ToU32(HexToRGBA(GetProperty("color")));

    float time = (float)CherryGUI::GetTime();

    CherryGUI::Dummy(ImVec2(300, 300));

    for (auto &p : m_Particles) {
      p.angle += 0.01f * p.speed * speed_mult;

      float dynamic_radius =
          p.radius + (sinf(time + p.angle * 2.0f) * amplitude);

      ImVec2 pos(center.x + cosf(p.angle) * dynamic_radius,
                 center.y + sinf(p.angle) * dynamic_radius);

      ImVec2 prev_pos(
          center.x + cosf(p.angle - 0.1f) * (dynamic_radius * 0.95f),
          center.y + sinf(p.angle - 0.1f) * (dynamic_radius * 0.95f));

      draw_list->AddLine(prev_pos, pos, color, thickness);

      draw_list->AddCircleFilled(pos, thickness * 1.5f,
                                 color & 0x00FFFFFF | 0x44000000);
    }
  }

  float GetPropertyAsFloat(const std::string &key) {
    return std::stof(GetProperty(key));
  }
};

} // namespace Components

namespace Kit {
inline Component &OrbitalVisualizer(const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::OrbitalVisualizer>(
      Application::GenerateUniqueID(label + "Orbital"));
}
} // namespace Kit
} // namespace Cherry

void Render() {
  static float speed = 1.0f;
  static float amp = 30.0f;
  static float thick = 1.5f;

  auto &viz = Cherry::Kit::OrbitalVisualizer("MainViz");

  CherryKit::InputFloat("Speed", &speed);
  CherryKit::InputFloat("Amplitude", &amp);
  CherryKit::InputFloat("Thickness", &thick);

  viz.SetProperty("speed_mult", std::to_string(speed));
  viz.SetProperty("amplitude", std::to_string(amp));
  viz.SetProperty("thickness", std::to_string(thick));
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}