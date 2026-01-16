#include "drawing.hpp"
#include "../app/app.hpp"
#include "../imgui/wrappers/wrappers.hpp"

namespace Cherry {
namespace Draw {

static inline ImVec2 ToIm(const Vec2 &v) { return ImVec2(v.x, v.y); }

static inline ImU32 Col(const std::string &hex) {
  return Cherry::HexToImU32(hex);
}

static ImU32 LerpColor(ImU32 a, ImU32 b, float t) {
  int r1 = (a >> 0) & 0xFF, g1 = (a >> 8) & 0xFF, b1 = (a >> 16) & 0xFF,
      a1 = (a >> 24) & 0xFF;
  int r2 = (b >> 0) & 0xFF, g2 = (b >> 8) & 0xFF, b2 = (b >> 16) & 0xFF,
      a2 = (b >> 24) & 0xFF;
  return IM_COL32(r1 + (int)((r2 - r1) * t), g1 + (int)((g2 - g1) * t),
                  b1 + (int)((b2 - b1) * t), a1 + (int)((a2 - a1) * t));
}

static void AddQuadGradientInternal(ImDrawList *drawList, ImVec2 p1, ImVec2 p2,
                                    ImVec2 p3, ImVec2 p4, ImU32 c1, ImU32 c2,
                                    ImU32 c3, ImU32 c4) {
  ImVec2 uv = ImGui::GetDrawListSharedData()->TexUvWhitePixel;

  drawList->PrimReserve(6, 4);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 1);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 3);

  drawList->PrimWriteVtx(p1, uv, c1);
  drawList->PrimWriteVtx(p2, uv, c2);
  drawList->PrimWriteVtx(p3, uv, c3);
  drawList->PrimWriteVtx(p4, uv, c4);
}

static void AddTriangleGradientInternal(ImDrawList *drawList, ImVec2 p1,
                                        ImVec2 p2, ImVec2 p3, ImU32 c1,
                                        ImU32 c2, ImU32 c3) {
  ImVec2 uv = ImGui::GetDrawListSharedData()->TexUvWhitePixel;

  drawList->PrimReserve(3, 3);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 1);
  drawList->PrimWriteIdx(drawList->_VtxCurrentIdx + 2);

  drawList->PrimWriteVtx(p1, uv, c1);
  drawList->PrimWriteVtx(p2, uv, c2);
  drawList->PrimWriteVtx(p3, uv, c3);
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

void CircleWindow(Vec2 center, float radius, const std::string &hexcol,
                  int num_segments) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddCircleFilled(
      {wp.x + center.x, wp.y + center.y}, radius, Col(hexcol), num_segments);
}

void CircleScreen(Vec2 center, float radius, const std::string &hexcol,
                  int num_segments) {
  ImGui::GetForegroundDrawList()->AddCircleFilled(ToIm(center), radius,
                                                  Col(hexcol), num_segments);
}

void CircleOutlineWindow(Vec2 center, float radius, const std::string &hexcol,
                         float thickness, int num_segments) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddCircle({wp.x + center.x, wp.y + center.y},
                                        radius, Col(hexcol), num_segments,
                                        thickness);
}
void CircleOutlineScreen(Vec2 center, float radius, const std::string &hexcol,
                         float thickness, int num_segments) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetForegroundDrawList()->AddCircle(
      {center.x, center.y}, radius, Col(hexcol), num_segments, thickness);
}

void TriangleWindow(Vec2 p1, Vec2 p2, Vec2 p3, const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddTriangleFilled(
      {wp.x + p1.x, wp.y + p1.y}, {wp.x + p2.x, wp.y + p2.y},
      {wp.x + p3.x, wp.y + p3.y}, Col(hexcol));
}

void TriangleScreen(Vec2 p1, Vec2 p2, Vec2 p3, const std::string &hexcol) {
  ImGui::GetForegroundDrawList()->AddTriangleFilled(ToIm(p1), ToIm(p2),
                                                    ToIm(p3), Col(hexcol));
}

void RectRoundedWindow(Vec2 pos, Vec2 size, float rounding,
                       const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddRectFilled(
      {wp.x + pos.x, wp.y + pos.y},
      {wp.x + pos.x + size.x, wp.y + pos.y + size.y}, Col(hexcol), rounding,
      ImDrawFlags_RoundCornersAll);
}

void RectOutlineRoundedWindow(Vec2 pos, Vec2 size, float rounding,
                              const std::string &hexcol, float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddRect(
      {wp.x + pos.x, wp.y + pos.y},
      {wp.x + pos.x + size.x, wp.y + pos.y + size.y}, Col(hexcol), rounding,
      ImDrawFlags_RoundCornersAll, thickness);
}
void RectOutlineRoundedScreen(Vec2 pos, Vec2 size, float rounding,
                              const std::string &hexcol, float thickness) {
  ImGui::GetForegroundDrawList()->AddRect(
      {pos.x, pos.y}, {pos.x + size.x, pos.y + size.y}, Col(hexcol), rounding,
      ImDrawFlags_RoundCornersAll, thickness);
}

void BeginRectMask(Vec2 pos, Vec2 size, bool intersect_with_current_clip_rect) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImVec2 min = {wp.x + pos.x, wp.y + pos.y};
  ImVec2 max = {wp.x + pos.x + size.x, wp.y + pos.y + size.y};

  ImGui::GetWindowDrawList()->PushClipRect(min, max,
                                           intersect_with_current_clip_rect);
}

void BeginRectMaskFixed(Vec2 pos, Vec2 size,
                        bool intersect_with_current_clip_rect) {
  ImVec2 min = {pos.x, pos.y};
  ImVec2 max = {pos.x + size.x, pos.y + size.y};

  ImGui::GetForegroundDrawList()->PushClipRect(
      min, max, intersect_with_current_clip_rect);
}

void EndRectMask() { ImGui::GetWindowDrawList()->PopClipRect(); }

void EndRectMaskFixed() { ImGui::GetForegroundDrawList()->PopClipRect(); }

void RectRoundedOutlineWindow(Vec2 pos, Vec2 size, float rounding,
                              const std::string &hexcol, float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddRect(
      {wp.x + pos.x, wp.y + pos.y},
      {wp.x + pos.x + size.x, wp.y + pos.y + size.y}, Col(hexcol), rounding,
      ImDrawFlags_RoundCornersAll, thickness);
}

void CircleSectorWindow(Vec2 center, float radius, float angle_min,
                        float angle_max, const std::string &hexcol,
                        int num_segments) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->PathArcTo({wp.x + center.x, wp.y + center.y},
                                        radius, angle_min, angle_max,
                                        num_segments);
  ImGui::GetWindowDrawList()->PathLineTo({wp.x + center.x, wp.y + center.y});
  ImGui::GetWindowDrawList()->PathFillConvex(Col(hexcol));
}
void RectRoundedScreen(Vec2 pos, Vec2 size, float rounding,
                       const std::string &hexcol) {
  ImGui::GetForegroundDrawList()->AddRectFilled(
      ToIm(pos), {pos.x + size.x, pos.y + size.y}, Col(hexcol), rounding,
      ImDrawFlags_RoundCornersAll);
}

void RectRoundedOutlineScreen(Vec2 pos, Vec2 size, float rounding,
                              const std::string &hexcol, float thickness) {
  ImGui::GetForegroundDrawList()->AddRect(
      ToIm(pos), {pos.x + size.x, pos.y + size.y}, Col(hexcol), rounding,
      ImDrawFlags_RoundCornersAll, thickness);
}

void CircleSectorScreen(Vec2 center, float radius, float angle_min,
                        float angle_max, const std::string &hexcol,
                        int num_segments) {
  ImDrawList *drawList = ImGui::GetForegroundDrawList();

  drawList->PathArcTo(ToIm(center), radius, angle_min, angle_max, num_segments);
  drawList->PathLineTo(ToIm(center));
  drawList->PathFillConvex(Col(hexcol));
}
void ImageWindow(Vec2 pos, Vec2 size, const std::string &path, Vec2 uv_min,
                 Vec2 uv_max, const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImVec2 p_min = {wp.x + pos.x, wp.y + pos.y};
  ImVec2 p_max = {p_min.x + size.x, p_min.y + size.y};

  ImGui::GetWindowDrawList()->AddImage(Cherry::GetTexture(path), p_min, p_max,
                                       {uv_min.x, uv_min.y},
                                       {uv_max.x, uv_max.y}, Col(hexcol));
}

void ImageScreen(Vec2 pos, Vec2 size, const std::string &path, Vec2 uv_min,
                 Vec2 uv_max, const std::string &hexcol) {
  ImVec2 p_min = {pos.x, pos.y};
  ImVec2 p_max = {pos.x + size.x, pos.y + size.y};

  ImGui::GetForegroundDrawList()->AddImage(Cherry::GetTexture(path), p_min,
                                           p_max, {uv_min.x, uv_min.y},
                                           {uv_max.x, uv_max.y}, Col(hexcol));
}
void ImageQuadWindow(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4,
                     const std::string &path, const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImGui::GetWindowDrawList()->AddImageQuad(
      Cherry::GetTexture(path), {wp.x + p1.x, wp.y + p1.y},
      {wp.x + p2.x, wp.y + p2.y}, {wp.x + p3.x, wp.y + p3.y},
      {wp.x + p4.x, wp.y + p4.y}, {0, 0}, {1, 0}, {1, 1}, {0, 1}, Col(hexcol));
}

void ImageQuadScreen(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4,
                     const std::string &path, const std::string &hexcol) {
  ImGui::GetForegroundDrawList()->AddImageQuad(
      Cherry::GetTexture(path), ToIm(p1), ToIm(p2), ToIm(p3), ToIm(p4), {0, 0},
      {1, 0}, {1, 1}, {0, 1}, Col(hexcol));
}

void ImageRoundedWindow(Vec2 pos, Vec2 size, const std::string &path,
                        float rounding, const std::string &hexcol) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImVec2 p_min = {wp.x + pos.x, wp.y + pos.y};
  ImVec2 p_max = {p_min.x + size.x, p_min.y + size.y};

  ImGui::GetWindowDrawList()->AddImageRounded(
      Cherry::GetTexture(path), p_min, p_max, {0, 0}, {1, 1}, Col(hexcol),
      rounding, ImDrawFlags_RoundCornersAll);
}

void ImageRoundedScreen(Vec2 pos, Vec2 size, const std::string &path,
                        float rounding, const std::string &hexcol) {
  ImVec2 p_min = {pos.x, pos.y};
  ImVec2 p_max = {pos.x + size.x, pos.y + size.y};

  ImGui::GetForegroundDrawList()->AddImageRounded(
      Cherry::GetTexture(path), p_min, p_max, {0, 0}, {1, 1}, Col(hexcol),
      rounding, ImDrawFlags_RoundCornersAll);
}

// --- RECT GRADIENT ---
void RectGradientWindow(Vec2 pos, Vec2 size, const std::string &col_start,
                        const std::string &col_end, bool vertical, int steps) {
  ImVec2 wp = ImGui::GetWindowPos();
  ImU32 c1 = Col(col_start);
  ImU32 c2 = Col(col_end);
  ImDrawList *dl = ImGui::GetWindowDrawList();

  for (int i = 0; i < steps; i++) {
    float t1 = (float)i / steps;
    float t2 = (float)(i + 1) / steps;
    ImU32 curr_col = LerpColor(c1, c2, t1);

    if (vertical)
      dl->AddRectFilled({wp.x + pos.x, wp.y + pos.y + size.y * t1},
                        {wp.x + pos.x + size.x, wp.y + pos.y + size.y * t2},
                        curr_col);
    else
      dl->AddRectFilled({wp.x + pos.x + size.x * t1, wp.y + pos.y},
                        {wp.x + pos.x + size.x * t2, wp.y + pos.y + size.y},
                        curr_col);
  }
}

void RectGradientScreen(Vec2 pos, Vec2 size, const std::string &col_start,
                        const std::string &col_end, bool vertical, int steps) {
  ImU32 c1 = Col(col_start);
  ImU32 c2 = Col(col_end);
  ImDrawList *dl = ImGui::GetForegroundDrawList();

  for (int i = 0; i < steps; i++) {
    float t1 = (float)i / steps;
    float t2 = (float)(i + 1) / steps;
    ImU32 curr_col = LerpColor(c1, c2, t1);

    if (vertical)
      dl->AddRectFilled({pos.x, pos.y + size.y * t1},
                        {pos.x + size.x, pos.y + size.y * t2}, curr_col);
    else
      dl->AddRectFilled({pos.x + size.x * t1, pos.y},
                        {pos.x + size.x * t2, pos.y + size.y}, curr_col);
  }
}

void RectGradientRoundedWindow(Vec2 pos, Vec2 size,
                               const std::string &hexcol_start,
                               const std::string &hexcol_end, float rounding,
                               bool vertical) {
  ImDrawList *dl = ImGui::GetWindowDrawList();
  ImVec2 wp = ImGui::GetWindowPos();

  ImVec2 p_min = {wp.x + pos.x, wp.y + pos.y};
  ImVec2 p_max = {wp.x + pos.x + size.x, wp.y + pos.y + size.y};

  ImU32 col1 = Col(hexcol_start);
  ImU32 col2 = Col(hexcol_end);

  int vtx_start = dl->VtxBuffer.Size;

  dl->AddRectFilled(p_min, p_max, IM_COL32_WHITE, rounding,
                    ImDrawFlags_RoundCornersAll);

  if (vertical) {
    ImGui::ShadeVertsLinearColorGradientKeepAlpha(
        dl, vtx_start, dl->VtxBuffer.Size, ImVec2(p_min.x, p_min.y),
        ImVec2(p_min.x, p_max.y), col1, col2);
  } else {
    ImGui::ShadeVertsLinearColorGradientKeepAlpha(
        dl, vtx_start, dl->VtxBuffer.Size, ImVec2(p_min.x, p_min.y),
        ImVec2(p_max.x, p_min.y), col1, col2);
  }
}

void RectGradientRoundedScreen(Vec2 pos, Vec2 size,
                               const std::string &hexcol_start,
                               const std::string &hexcol_end, float rounding,
                               bool vertical) {
  ImDrawList *dl = ImGui::GetForegroundDrawList();

  ImVec2 p_min = {pos.x, pos.y};
  ImVec2 p_max = {pos.x + size.x, pos.y + size.y};

  ImU32 col1 = Col(hexcol_start);
  ImU32 col2 = Col(hexcol_end);

  int vtx_start = dl->VtxBuffer.Size;

  dl->AddRectFilled(p_min, p_max, IM_COL32_WHITE, rounding,
                    ImDrawFlags_RoundCornersAll);

  if (vertical) {
    ImGui::ShadeVertsLinearColorGradientKeepAlpha(
        dl, vtx_start, dl->VtxBuffer.Size, ImVec2(p_min.x, p_min.y),
        ImVec2(p_min.x, p_max.y), col1, col2);
  } else {
    ImGui::ShadeVertsLinearColorGradientKeepAlpha(
        dl, vtx_start, dl->VtxBuffer.Size, ImVec2(p_min.x, p_min.y),
        ImVec2(p_max.x, p_min.y), col1, col2);
  }
}

void CircleGradientWindow(Vec2 center, float radius,
                          const std::string &col_center,
                          const std::string &col_edge, int steps) {
  ImVec2 wp = ImGui::GetWindowPos();
  CircleGradientScreen({wp.x + center.x, wp.y + center.y}, radius, col_center,
                       col_edge, steps);
}

void CircleGradientScreen(Vec2 center, float radius,
                          const std::string &col_center,
                          const std::string &col_edge, int steps) {
  ImU32 c1 = Col(col_center);
  ImU32 c2 = Col(col_edge);
  ImDrawList *dl = ImGui::GetForegroundDrawList();
  for (int i = steps; i > 0; i--) {
    float t = (float)i / steps;
    dl->AddCircleFilled(ToIm(center), radius * t, LerpColor(c1, c2, t), 32);
  }
}

void TriangleGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, const std::string &c1,
                            const std::string &c2, const std::string &c3) {
  ImVec2 wp = ImGui::GetWindowPos();
  AddTriangleGradientInternal(
      ImGui::GetWindowDrawList(), {wp.x + p1.x, wp.y + p1.y},
      {wp.x + p2.x, wp.y + p2.y}, {wp.x + p3.x, wp.y + p3.y}, Col(c1), Col(c2),
      Col(c3));
}

void TriangleGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, const std::string &c1,
                            const std::string &c2, const std::string &c3) {
  AddTriangleGradientInternal(ImGui::GetForegroundDrawList(), ToIm(p1),
                              ToIm(p2), ToIm(p3), Col(c1), Col(c2), Col(c3));
}

void QuadGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4,
                        const std::string &c1, const std::string &c2,
                        const std::string &c3, const std::string &c4) {
  ImVec2 wp = ImGui::GetWindowPos();
  AddQuadGradientInternal(
      ImGui::GetWindowDrawList(), {wp.x + p1.x, wp.y + p1.y},
      {wp.x + p2.x, wp.y + p2.y}, {wp.x + p3.x, wp.y + p3.y},
      {wp.x + p4.x, wp.y + p4.y}, Col(c1), Col(c2), Col(c3), Col(c4));
}

void QuadGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4,
                        const std::string &c1, const std::string &c2,
                        const std::string &c3, const std::string &c4) {
  AddQuadGradientInternal(ImGui::GetForegroundDrawList(), ToIm(p1), ToIm(p2),
                          ToIm(p3), ToIm(p4), Col(c1), Col(c2), Col(c3),
                          Col(c4));
}

void RectOutlineGradientWindow(Vec2 pos, Vec2 size,
                               const std::string &col_start,
                               const std::string &col_end, float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  RectOutlineGradientScreen({wp.x + pos.x, wp.y + pos.y}, size, col_start,
                            col_end, thickness);
}

void RectOutlineGradientScreen(Vec2 pos, Vec2 size,
                               const std::string &col_start,
                               const std::string &col_end, float thickness) {
  ImU32 c1 = Col(col_start);
  ImU32 c2 = Col(col_end);
  ImDrawList *dl = ImGui::GetForegroundDrawList();
  ImVec2 p[4] = {{pos.x, pos.y},
                 {pos.x + size.x, pos.y},
                 {pos.x + size.x, pos.y + size.y},
                 {pos.x, pos.y + size.y}};
  dl->AddLine(p[0], p[1], c1, thickness);
  dl->AddLine(p[1], p[2], LerpColor(c1, c2, 0.5f), thickness);
  dl->AddLine(p[2], p[3], c2, thickness);
  dl->AddLine(p[3], p[0], LerpColor(c2, c1, 0.5f), thickness);
}

void CircleOutlineGradientWindow(Vec2 center, float radius,
                                 const std::string &col_start,
                                 const std::string &col_end, float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  CircleOutlineGradientScreen({wp.x + center.x, wp.y + center.y}, radius,
                              col_start, col_end, thickness);
}

void CircleOutlineGradientScreen(Vec2 center, float radius,
                                 const std::string &col_start,
                                 const std::string &col_end, float thickness) {
  ImU32 c1 = Col(col_start);
  ImU32 c2 = Col(col_end);
  ImDrawList *dl = ImGui::GetForegroundDrawList();
  const int segments = 64;
  for (int i = 0; i < segments; i++) {
    float a1 = ((float)i / segments) * 6.28318f;
    float a2 = ((float)(i + 1) / segments) * 6.28318f;
    dl->AddLine({center.x + cosf(a1) * radius, center.y + sinf(a1) * radius},
                {center.x + cosf(a2) * radius, center.y + sinf(a2) * radius},
                LerpColor(c1, c2, (float)i / segments), thickness);
  }
}

void TriangleOutlineGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3,
                                   const std::string &c1, const std::string &c2,
                                   float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  TriangleOutlineGradientScreen({wp.x + p1.x, wp.y + p1.y},
                                {wp.x + p2.x, wp.y + p2.y},
                                {wp.x + p3.x, wp.y + p3.y}, c1, c2, thickness);
}

void TriangleOutlineGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3,
                                   const std::string &c1, const std::string &c2,
                                   float thickness) {
  ImU32 col1 = Col(c1);
  ImU32 col2 = Col(c2);
  ImDrawList *dl = ImGui::GetForegroundDrawList();
  dl->AddLine(ToIm(p1), ToIm(p2), col1, thickness);
  dl->AddLine(ToIm(p2), ToIm(p3), LerpColor(col1, col2, 0.5f), thickness);
  dl->AddLine(ToIm(p3), ToIm(p1), col2, thickness);
}

void QuadOutlineGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4,
                               const std::string &c1, const std::string &c2,
                               float thickness) {
  ImVec2 wp = ImGui::GetWindowPos();
  QuadOutlineGradientScreen({wp.x + p1.x, wp.y + p1.y},
                            {wp.x + p2.x, wp.y + p2.y},
                            {wp.x + p3.x, wp.y + p3.y},
                            {wp.x + p4.x, wp.y + p4.y}, c1, c2, thickness);
}

void QuadOutlineGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4,
                               const std::string &c1, const std::string &c2,
                               float thickness) {
  ImU32 col1 = Col(c1);
  ImU32 col2 = Col(c2);
  ImDrawList *dl = ImGui::GetForegroundDrawList();
  dl->AddLine(ToIm(p1), ToIm(p2), col1, thickness);
  dl->AddLine(ToIm(p2), ToIm(p3), col1, thickness);
  dl->AddLine(ToIm(p3), ToIm(p4), col2, thickness);
  dl->AddLine(ToIm(p4), ToIm(p1), col2, thickness);
}
} // namespace Draw
} // namespace Cherry
