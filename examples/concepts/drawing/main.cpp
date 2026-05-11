#include "../../../cherry.hpp"

void Render() {
  float dt = Cherry::GetDeltaTime();
  static float t = 0.0f;
  t += dt;

  const float W = 680.f, H = 460.f;

  // bg
  Cherry::Drawing::RectGradientWindow({ 0, 0 }, { W, H }, "#0D0D14FF", "#1A1030FF", true);

  // Animated lines
  float cx1 = 90.f, cy1 = 90.f;
  for (int i = 0; i < 12; i++) {
    float a = (float)i / 12.f * M_PI * 2.f + t * 0.4f;
    float hue = (float)i / 12.f * 360.f;
    float thick = 1.5f + sinf(t * 2.f + i) * 0.8f;
    std::string col = Cherry::HslaHex(hue, 0.8f, 0.65f, 1.0f);
    Cherry::Drawing::LineWindow({ cx1, cy1 }, { cx1 + cosf(a) * 55.f, cy1 + sinf(a) * 55.f }, col, thick);
  }

  // Pulsed circles
  float puls = 2.f + sinf(t * 1.8f) * 1.2f;
  for (int i = 6; i >= 1; i--) {
    float alpha = (float)i / 7.f * 0.6f;
    std::string col = Cherry::HslaHex(200.f, 0.9f, 0.70f, alpha);  // around #64C8FF
    Cherry::Drawing::CircleOutlineWindow({ 240, 90 }, i * 10.f * puls / 2.f, col, 1.f + i * 0.3f);
  }

  // Colored wheel
  for (int i = 0; i < 8; i++) {
    float a0 = (float)i / 8.f * M_PI * 2.f - M_PI * 0.5f;
    float a1 = a0 + (M_PI * 2.f / 8.f) * 0.78f;
    float pulse = 0.5f + 0.5f * sinf(t * 2.f - i * 0.8f);
    std::string col = Cherry::HslaHex(180.f + i * 30.f, 0.7f, 0.6f, 0.5f + 0.5f * pulse);
    Cherry::Drawing::CircleSectorWindow({ 400, 90 }, 55.f * (0.6f + 0.4f * pulse), a0, a1, col);
  }

  // Triangles
  float ang = t * 0.7f;
  float cx4 = 560.f, cy4 = 90.f, sz = 48.f;
  for (int i = 0; i < 3; i++) {
    float a0 = (float)i / 3.f * M_PI * 2.f + ang;
    float a1 = a0 + M_PI * 2.f / 3.f;
    float a2 = a0 + M_PI * 4.f / 3.f;
    std::string c1 = Cherry::HslaHex(i * 120.f, 0.8f, 0.60f);
    std::string c2 = Cherry::HslaHex(i * 120.f + 60.f, 0.8f, 0.70f);
    std::string c3 = Cherry::HslaHex(i * 120.f + 120.f, 0.8f, 0.50f);
    Cherry::Drawing::TriangleGradientWindow(
        { cx4 + cosf(a0) * sz, cy4 + sinf(a0) * sz },
        { cx4 + cosf(a1) * sz, cy4 + sinf(a1) * sz },
        { cx4 + cosf(a2) * sz, cy4 + sinf(a2) * sz },
        c1,
        c2,
        c3);
  }

  // Color palete
  const int gcols = 14, grows = 3;
  const float gsz = 40.f, ggap = 6.f;
  for (int row = 0; row < grows; row++) {
    for (int col = 0; col < gcols; col++) {
      float phase = col * 0.4f + row * 1.2f + t * 2.f;
      float v = 0.5f + 0.5f * sinf(phase);
      std::string color = Cherry::HslaHex(fmodf(col * 25.f + t * 30.f, 360.f), 0.7f, 0.40f + v * 0.30f, 0.6f + v * 0.40f);
      Cherry::Drawing::RectRoundedWindow(
          { 20.f + col * (gsz + ggap), 170.f + row * (gsz + ggap) }, { gsz, gsz }, 8.f, color);
    }
  }

  // Double sinusoidal polyline
  std::vector<Cherry::Vec2> pts, pts2;
  for (int x = 0; x <= (int)W; x += 5) {
    float y = 310.f + sinf(x * 0.02f + t * 3.f) * 28.f + sinf(x * 0.05f - t * 2.f) * 14.f;
    float y2 = 310.f + sinf(x * 0.03f - t * 2.5f) * 22.f + cosf(x * 0.04f + t * 1.5f) * 10.f;
    pts.push_back({ (float)x, y });
    pts2.push_back({ (float)x, y2 });
  }
  Cherry::Drawing::PolylineWindow(pts, "#64FFB4FF", 2.0f);
  Cherry::Drawing::PolylineWindow(pts2, "#FFB450FF", 1.5f);

  // Mask with gradient circles
  Cherry::Drawing::BeginRectMask({ 20, 345 }, { W - 40, 90 });
  for (int i = 0; i < 7; i++) {
    float bx = 20.f + fmodf(t * 60.f * (i % 2 ? 1.f : -1.f) + i * (W / 6.f), W + 80.f) - 40.f;
    std::string cIn = Cherry::HslaHex(fmodf(i * 50.f + t * 20.f, 360.f), 0.8f, 0.7f, 0.6f);
    std::string cOut = Cherry::HslaHex(fmodf(i * 50.f + t * 20.f, 360.f), 0.6f, 0.4f, 0.0f);
    Cherry::Drawing::CircleGradientWindow({ bx, 345 + 45 }, 35.f + i * 6.f, cIn, cOut);
  }
  Cherry::Drawing::EndRectMask();

  // Crosses
  for (int i = 0; i < 5; i++) {
    float bx = 60.f + i * 140.f;
    float by = 430.f;
    float csz = 10.f + fabsf(sinf(t * 1.5f + i)) * 8.f;
    std::string col = Cherry::HslaHex(fmodf(i * 60.f + t * 40.f, 360.f), 0.8f, 0.7f, 1.0f);
    Cherry::Drawing::LineHWindow(by, bx - csz, bx + csz, col, 2.f);
    Cherry::Drawing::LineVWindow(bx, by - csz, by + csz, col, 2.f);
  }

  // Text
  std::string aCol = Cherry::HslaHex(240.f, 0.35f, 0.78f, 0.6f + 0.4f * sinf(t * 2.f));
  Cherry::Drawing::TextWindow("Cherry::Drawing", { 478, 14 }, 13.f, aCol);
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);
  config.SetDefaultHeight(460.0f);
  config.SetDefaultWidth(680.0f);
  config.DisableResize = true;
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}