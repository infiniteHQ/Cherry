#include <vulkan/vulkan.h>

#include <algorithm>
#include <cherry.hpp>
#include <cmath>
#include <cstring>
#include <string>

struct MyParams {
  float time = 0.f;
  float scale = 3.f;
  float intensity = 1.f;
  float colorR = 0.2f;
  float colorG = 0.5f;
  float colorB = 1.0f;
  int effectType = 8;
  float param1 = 1.5f;  // octaves / warp strength / ring density
  float camX = 0.f;
  float camY = 0.f;
};
static_assert(sizeof(MyParams) <= 128, "MyParams exceeds Vulkan push-constant limit (128 bytes)");

static MyParams g_Params{};
static float g_Speed = 1.f;
static bool g_Animate = true;
static bool g_Dragging = false;
static ImVec2 g_DragStart{ 0, 0 };
static Cherry::Shaders::ComputeContext g_Compute;

struct EffectInfo {
  const char* name;
  const char* param1Label;  // label for the "param1" slider
  float param1Min;
  float param1Max;
  const char* tooltip;
};

static const EffectInfo k_Effects[] = {
  { "0 - Raw Perlin", "–", 0.f, 5.f, "Single-octave gradient noise." },
  { "1 - fBm Classic", "Octaves", 0.f, 5.f, "Fractal Brownian Motion – stacks Perlin octaves." },
  { "2 - Ridged fBm", "Octaves", 0.f, 5.f, "Inverted ridged version of fBm for mountain-like forms." },
  { "3 - Domain Warp", "Warp Strength", 0.f, 5.f, "fBm whose domain is itself warped by another fBm." },
  { "4 - Worley / Cellular", "–", 0.f, 5.f, "Distance to nearest random feature point." },
  { "5 - Worley + fBm", "Octaves", 0.f, 5.f, "Blend between Worley and fBm." },
  { "6 - Curl Flow", "Curl Strength", 0.f, 5.f, "Divergence-free curl noise for flow-field look." },
  { "7 - Marble", "Distortion", 0.f, 5.f, "Sine pattern distorted by fBm to mimic marble veins." },
  { "8 - Wood Rings", "Ring Density", 0.f, 5.f, "Concentric rings distorted by fBm." },
  { "9 - Turbulence", "Octaves", 0.f, 5.f, "Absolute-value fBm for cloudy/fire turbulence." },
  { "10 - Swiss Cheese", "Threshold", 0.f, 5.f, "Value noise blob-threshold pattern." },
};
static constexpr int k_EffectCount = (int)(sizeof(k_Effects) / sizeof(k_Effects[0]));

static void LoadShader(const std::string& path) {
  if (path.empty())
    return;
  if (g_Compute.initialized) {
    Cherry::Shaders::CleanupVulkanCompute(&g_Compute);
    g_Compute = Cherry::Shaders::ComputeContext{};
  }
  g_Compute.shaderPath = path;
  g_Compute.width = 512;
  g_Compute.height = 512;
  Cherry::Shaders::InitVulkanCompute(g_Params, &g_Compute);
}

static void RenderPreview() {
  if (!g_Compute.initialized)
    LoadShader(Cherry::GetPath("shaders/shader.comp.spv"));

  float dt = ImGui::GetIO().DeltaTime;
  if (g_Animate)
    g_Params.time += dt * g_Speed;

  Cherry::Shaders::ExecuteShader(g_Params, &g_Compute);

  ImVec2 avail = ImGui::GetContentRegionAvail();
  float size = (std::min)(avail.x, avail.y - 30.f);
  if (size < 80.f)
    size = 80.f;

  if (g_Compute.imguiTextureID) {
    ImVec2 imgPos = ImGui::GetCursorScreenPos();
    ImGui::Image(g_Compute.imguiTextureID, ImVec2(size, size));

    ImVec2 mp = ImGui::GetMousePos();
    bool over = mp.x >= imgPos.x && mp.x < imgPos.x + size && mp.y >= imgPos.y && mp.y < imgPos.y + size;
    if (over && ImGui::IsMouseClicked(0)) {
      g_Dragging = true;
      g_DragStart = mp;
    }
    if (g_Dragging) {
      if (ImGui::IsMouseDown(0)) {
        g_Params.camX = std::clamp(g_Params.camX + (mp.x - g_DragStart.x) / size * 0.4f, -2.f, 2.f);
        g_Params.camY = std::clamp(g_Params.camY + (mp.y - g_DragStart.y) / size * 0.3f, -2.f, 2.f);
        g_DragStart = mp;
      } else {
        g_Dragging = false;
      }
    }
  } else {
    ImGui::TextDisabled("Initializing GPU...");
  }

  ImGui::Spacing();
  ImGui::Text("%.0f FPS  |  %dx%d  |  t=%.1fs", ImGui::GetIO().Framerate, g_Compute.width, g_Compute.height, g_Params.time);
}

static void RenderParameters() {
  CherryKit::SeparatorText("Noise Type");

  const char* currentName =
      (g_Params.effectType >= 0 && g_Params.effectType < k_EffectCount) ? k_Effects[g_Params.effectType].name : "?";
  if (ImGui::BeginCombo("Effect", currentName)) {
    for (int i = 0; i < k_EffectCount; i++) {
      bool selected = (g_Params.effectType == i);
      if (ImGui::Selectable(k_Effects[i].name, selected))
        g_Params.effectType = i;
      if (selected)
        ImGui::SetItemDefaultFocus();
      if (ImGui::IsItemHovered())
        ImGui::SetTooltip("%s", k_Effects[i].tooltip);
    }
    ImGui::EndCombo();
  }

  ImGui::Spacing();
  for (int i = 0; i < k_EffectCount; i++) {
    if (i % 4 != 0)
      ImGui::SameLine();
    char label[8];
    snprintf(label, sizeof(label), " %d ", i);
    bool active = (g_Params.effectType == i);
    if (active) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 1.f));
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.36f, 0.69f, 1.00f, 1.f));
    }
    if (ImGui::Button(label))
      g_Params.effectType = i;
    if (active)
      ImGui::PopStyleColor(2);
    if (ImGui::IsItemHovered())
      ImGui::SetTooltip("%s", k_Effects[i].name);
  }

  if (g_Params.effectType >= 0 && g_Params.effectType < k_EffectCount) {
    ImGui::Spacing();
    ImGui::TextDisabled("%s", k_Effects[g_Params.effectType].tooltip);
  }

  ImGui::Spacing();
  CherryKit::SeparatorText("Noise Parameters");

  ImGui::SliderFloat("Scale", &g_Params.scale, 0.5f, 20.f);
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Zoom level of the noise pattern.");

  ImGui::SliderFloat("Intensity", &g_Params.intensity, 0.1f, 3.f);
  if (ImGui::IsItemHovered())
    ImGui::SetTooltip("Contrast / brightness multiplier.");

  // param1 label changes per effect
  const EffectInfo& ei = k_Effects[std::clamp(g_Params.effectType, 0, k_EffectCount - 1)];
  if (strcmp(ei.param1Label, "–") != 0) {
    ImGui::SliderFloat(ei.param1Label, &g_Params.param1, ei.param1Min, ei.param1Max);
  }

  ImGui::Spacing();
  CherryKit::SeparatorText("Animation");

  ImGui::Checkbox("Animate", &g_Animate);
  if (g_Animate) {
    ImGui::SameLine();
    ImGui::SetNextItemWidth(120.f);
    ImGui::SliderFloat("Speed", &g_Speed, 0.0f, 5.0f);
  }

  ImGui::Spacing();
  CherryKit::SeparatorText("Color");
  ImGui::ColorEdit3("Base Color", &g_Params.colorR);

  ImGui::Spacing();
  struct Preset {
    const char* name;
    float r, g, b;
  };
  static const Preset presets[] = {
    { "Ocean", 0.05f, 0.37f, 0.70f }, { "Lava", 0.85f, 0.20f, 0.05f }, { "Forest", 0.10f, 0.55f, 0.20f },
    { "Ghost", 0.80f, 0.80f, 0.90f }, { "Gold", 0.95f, 0.75f, 0.10f }, { "Toxic", 0.30f, 0.90f, 0.20f },
  };
  for (int i = 0; i < (int)(sizeof(presets) / sizeof(presets[0])); i++) {
    if (i % 3 != 0)
      ImGui::SameLine();
    ImVec4 pc{ presets[i].r * 0.7f + 0.1f, presets[i].g * 0.7f + 0.1f, presets[i].b * 0.7f + 0.1f, 1.f };
    ImGui::PushStyleColor(ImGuiCol_Button, pc);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(pc.x + 0.1f, pc.y + 0.1f, pc.z + 0.1f, 1.f));
    if (ImGui::Button(presets[i].name, ImVec2(70, 0))) {
      g_Params.colorR = presets[i].r;
      g_Params.colorG = presets[i].g;
      g_Params.colorB = presets[i].b;
    }
    ImGui::PopStyleColor(2);
  }

  ImGui::Spacing();
  CherryKit::SeparatorText("Camera");
  ImGui::Text("Pan  X=%.2f  Y=%.2f  (drag image)", g_Params.camX, g_Params.camY);
  ImGui::Text("Tip: drag the preview image to pan.");

  ImGui::Spacing();
  if (ImGui::Button("Reset Camera")) {
    g_Params.camX = 0.f;
    g_Params.camY = 0.f;
    g_Params.time = 0.f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Reload Shader"))
    LoadShader(Cherry::GetPath("shaders/shader.comp.spv"));

  ImGui::Spacing();
  CherryKit::SeparatorText("Info");
  ImGui::TextDisabled("param1 drives octave count on fBm effects.");
  ImGui::TextDisabled("Octaves = round(param1 × 2)  →  [0-10]");
}

CherryApplication CherryMain(int argc, char** argv) {
  CherryAppConfig config;
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);
  config.SetName("Perlin Noise Playground");

  auto app = new CherryApplication(config);
  Cherry::AddAppWindow(CherryKit::WindowSimple("Preview", RenderPreview));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Parameters", RenderParameters));
  return app;
}

int main(int argc, char* argv[]) {
  CherryRun(argc, argv);
  Cherry::Shaders::CleanupVulkanCompute(&g_Compute);
  return 0;
}