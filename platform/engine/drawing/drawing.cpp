#include "drawing.hpp"
#include "../imgui/wrappers/wrappers.hpp"

namespace Cherry {
namespace Draw {

static inline ImVec2 ToIm(const Vec2 &v) { return ImVec2(v.x, v.y); }

static inline ImU32 Col(const std::string &hex) {
  return Cherry::HexToImU32(hex);
}

void LineWindow(Vec2 a, Vec2 b, const std::string &hexcol, float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddLine({wp.x + a.x, wp.y + a.y},
                                      {wp.x + b.x, wp.y + b.y}, Col(hexcol),
                                      thickness);
}

void LineScreen(Vec2 a, Vec2 b, const std::string &hexcol, float thickness) {
  ImGui::GetForegroundDrawList()->AddLine(ToIm(a), ToIm(b), Col(hexcol),
                                          thickness);
}

void LineHWindow(float y, float x0, float x1, const std::string &hexcol,
                 float thickness) {
  LineWindow({x0, y}, {x1, y}, hexcol, thickness);
}

void LineVWindow(float x, float y0, float y1, const std::string &hexcol,
                 float thickness) {
  LineWindow({x, y0}, {x, y1}, hexcol, thickness);
}

void LineHScreen(float y, float x0, float x1, const std::string &hexcol,
                 float thickness) {
  LineScreen({x0, y}, {x1, y}, hexcol, thickness);
}

void LineVScreen(float x, float y0, float y1, const std::string &hexcol,
                 float thickness) {
  LineScreen({x, y0}, {x, y1}, hexcol, thickness);
}

void PolylineWindow(const std::vector<Vec2> &pts, const std::string &hexcol,
                    float thickness) {
  if (pts.size() < 2)
    return;

  ImVec2 wp = ImGui::GetWindowPos();
  ImDrawList *dl = ImGui::GetWindowDrawList();

  for (size_t i = 0; i + 1 < pts.size(); ++i) {
    dl->AddLine({wp.x + pts[i].x, wp.y + pts[i].y},
                {wp.x + pts[i + 1].x, wp.y + pts[i + 1].y}, Col(hexcol),
                thickness);
  }
}

void PolylineScreen(const std::vector<Vec2> &pts, const std::string &hexcol,
                    float thickness) {
  if (pts.size() < 2)
    return;

  ImDrawList *dl = ImGui::GetForegroundDrawList();

  for (size_t i = 0; i + 1 < pts.size(); ++i) {
    dl->AddLine(ToIm(pts[i]), ToIm(pts[i + 1]), Col(hexcol), thickness);
  }
}

void CrossWindow(Vec2 c, float size, const std::string &hexcol,
                 float thickness) {
  float h = size * 0.5f;
  LineWindow({c.x - h, c.y}, {c.x + h, c.y}, hexcol, thickness);
  LineWindow({c.x, c.y - h}, {c.x, c.y + h}, hexcol, thickness);
}

void CrossScreen(Vec2 c, float size, const std::string &hexcol,
                 float thickness) {
  float h = size * 0.5f;
  LineScreen({c.x - h, c.y}, {c.x + h, c.y}, hexcol, thickness);
  LineScreen({c.x, c.y - h}, {c.x, c.y + h}, hexcol, thickness);
}

void RectWindow(Vec2 pos, Vec2 size, const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddRectFilled(
      {wp.x + pos.x, wp.y + pos.y},
      {wp.x + pos.x + size.x, wp.y + pos.y + size.y}, Col(hexcol));
}

void RectScreen(Vec2 pos, Vec2 size, const std::string &hexcol) {
  ImGui::GetForegroundDrawList()->AddRectFilled(
      ToIm(pos), {pos.x + size.x, pos.y + size.y}, Col(hexcol));
}

void RectOutlineWindow(Vec2 pos, Vec2 size, const std::string &hexcol,
                       float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddRect(
      {wp.x + pos.x, wp.y + pos.y},
      {wp.x + pos.x + size.x, wp.y + pos.y + size.y}, Col(hexcol), 0.0f, 0,
      thickness);
}

void RectOutlineScreen(Vec2 pos, Vec2 size, const std::string &hexcol,
                       float thickness) {
  ImGui::GetForegroundDrawList()->AddRect(ToIm(pos),
                                          {pos.x + size.x, pos.y + size.y},
                                          Col(hexcol), 0.0f, 0, thickness);
}

void SquareWindow(Vec2 pos, float size, const std::string &hexcol) {
  RectWindow(pos, {size, size}, hexcol);
}

void SquareScreen(Vec2 pos, float size, const std::string &hexcol) {
  RectScreen(pos, {size, size}, hexcol);
}

void QuadWindow(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImDrawList *dl = ImGui::GetWindowDrawList();

  dl->AddQuadFilled({wp.x + p0.x, wp.y + p0.y}, {wp.x + p1.x, wp.y + p1.y},
                    {wp.x + p2.x, wp.y + p2.y}, {wp.x + p3.x, wp.y + p3.y},
                    Col(hexcol));
}

void QuadScreen(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const std::string &hexcol) {
  ImGui::GetForegroundDrawList()->AddQuadFilled(ToIm(p0), ToIm(p1), ToIm(p2),
                                                ToIm(p3), Col(hexcol));
}
void TextWindow(const std::string &text, Vec2 pos, float size,
                const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImDrawList *dl = ImGui::GetWindowDrawList();

  float currentFontSize = ImGui::GetFontSize();
  float scale = size / currentFontSize;

  dl->AddText(ImGui::GetFont(), size, {wp.x + pos.x, wp.y + pos.y}, Col(hexcol),
              text.c_str());
}

void TextScreen(const std::string &text, Vec2 pos, float size,
                const std::string &hexcol) {
  ImDrawList *dl = ImGui::GetForegroundDrawList();

  dl->AddText(ImGui::GetFont(), size, ToIm(pos), Col(hexcol), text.c_str());
}
} // namespace Draw
} // namespace Cherry
