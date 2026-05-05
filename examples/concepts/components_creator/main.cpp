#include <cmath>
#include <vector>

#include "../../../cherry.hpp"

void Render() {
  Cherry::Use("OrbitalVisualizer", []() {
    {
      auto &cmp = CherryCurrentComponent;

      struct Particle {
        float angle;
        float radius;
        float speed;
      };

      static bool init = false;
      static std::vector<Particle> particles;
      static const int count = 100;

      float speed_mult = std::stof(cmp.GetProperty("speed_mult"));
      float amplitude = std::stof(cmp.GetProperty("amplitude"));
      float thickness = std::stof(cmp.GetProperty("thickness"));

      if (!init) {
        for (int i = 0; i < count; i++) {
          particles.push_back({ (float)i * (6.28f / count), 20.0f + (i * 0.5f), 0.5f + ((float)rand() / RAND_MAX) });
        }
        init = true;
      }

      ImDrawList *draw_list = CherryGUI::GetWindowDrawList();
      ImVec2 center = CherryGUI::GetCursorScreenPos() + ImVec2(150, 150);

      ImU32 color = CherryGUI::ColorConvertFloat4ToU32(Cherry::HexToRGBA(cmp.GetProperty("color")));

      float time = (float)CherryGUI::GetTime();

      CherryGUI::Dummy(ImVec2(300, 300));

      for (auto &p : particles) {
        p.angle += 0.01f * p.speed * speed_mult;

        float dynamic_radius = p.radius + (sinf(time + p.angle * 2.0f) * amplitude);

        ImVec2 pos(center.x + cosf(p.angle) * dynamic_radius, center.y + sinf(p.angle) * dynamic_radius);

        ImVec2 prev_pos(
            center.x + cosf(p.angle - 0.1f) * (dynamic_radius * 0.95f),
            center.y + sinf(p.angle - 0.1f) * (dynamic_radius * 0.95f));

        draw_list->AddLine(prev_pos, pos, color, thickness);

        draw_list->AddCircleFilled(pos, thickness * 1.5f, color & 0x00FFFFFF | 0x44000000);
      }
    }
  });

  static float speed = 1.0f;
  static float amp = 30.0f;
  static float thick = 1.5f;
  static std::string color = "#00F2FF";

  auto &vizd = Cherry::Draw(
      "OrbitalVisualizer",
      {
          { "id", std::string("viz") },
          { "prop:speed_mult", 1.0f },
          { "prop:amplitude", 50.0f },
          { "prop:thickness", 2.0f },
          { "prop:color", "#00F2FF" },
      });

  CherryKit::TableSimple(
      "Parameters",
      {
          CherryKit::KeyValFloat("Speed", &speed),
          CherryKit::KeyValFloat("Amplitude", &amp),
          CherryKit::KeyValFloat("Thickness", &thick),
          CherryKit::KeyValColorHex("Color", &color),
      });

  auto &viz = CherryApp.GetComponent(CherryID("viz"));
  vizd.SetProperty("speed_mult", speed);
  vizd.SetProperty("amplitude", amp);
  vizd.SetProperty("color", color);
  vizd.SetProperty("thickness", thick);
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);
  config.SetDefaultWidth(500);
  config.SetDefaultHeight(500);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}