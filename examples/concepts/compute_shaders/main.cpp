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
  float param1 = 1.5f;
  float camX = 0.f;
  float camY = 0.f;
};

static_assert(sizeof(MyParams) <= 128, "MyParams exceed Vulkan constants size ! (128 bytes)");

static MyParams g_Params{};
static float g_Speed = 1.f;
static bool g_Animate = true;
static bool g_Dragging = false;
static ImVec2 g_DragStart{ 0, 0 };

static Cherry::Shaders::ComputeContext g_Compute;

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
  float size = std::min(avail.x, avail.y - 30.f);
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
        g_Params.camX = std::clamp(g_Params.camX + (mp.x - g_DragStart.x) / size * .4f, -1.f, 1.f);
        g_Params.camY = std::clamp(g_Params.camY + (mp.y - g_DragStart.y) / size * .3f, -.8f, .8f);
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
  ImGui::SliderFloat("Speed", &g_Speed, 0.1f, 5.0f);
  ImGui::SliderFloat("Scale", &g_Params.scale, 0.5f, 10.f);
  ImGui::SliderFloat("Intensity", &g_Params.intensity, 0.1f, 3.0f);
  ImGui::SliderFloat("Param1", &g_Params.param1, 0.0f, 5.0f);
  ImGui::SliderInt("Effect", &g_Params.effectType, 0, 10);

  ImGui::Separator();

  ImGui::Checkbox("Animate", &g_Animate);
  ImGui::ColorEdit3("Color", &g_Params.colorR);

  ImGui::Separator();

  ImGui::Text("Camera  X=%.2f  Y=%.2f", g_Params.camX, g_Params.camY);
  if (ImGui::Button("Reset Camera")) {
    g_Params.camX = 0.f;
    g_Params.camY = 0.f;
    g_Params.time = 0.f;
  }
  ImGui::SameLine();
  if (ImGui::Button("Reload Shader"))
    LoadShader(Cherry::GetPath("shaders/shader.comp.spv"));
}

CherryApplication CherryMain(int argc, char** argv) {
  CherryAppConfig config;
  config.SetRenderMode(Cherry::WindowRenderingMethod::DockingWindows);
  config.SetName("GPU Shader Playground");

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