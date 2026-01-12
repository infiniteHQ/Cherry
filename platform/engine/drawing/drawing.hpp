#pragma once
#include <string>
#include <vector>

#include "../../../src/utils/types.hpp"

namespace Cherry {
namespace Draw {
void LineWindow(Vec2 start, Vec2 end, const std::string &hexcol,
                float thickness = 1.0f);
void LineScreen(Vec2 start, Vec2 end, const std::string &hexcol,
                float thickness = 1.0f);
void LineHWindow(float y, float x_start, float x_end, const std::string &hexcol,
                 float thickness = 1.0f);
void LineVWindow(float x, float y_start, float y_end, const std::string &hexcol,
                 float thickness = 1.0f);
void LineHScreen(float y, float x_start, float x_end, const std::string &hexcol,
                 float thickness = 1.0f);
void LineVScreen(float x, float y_start, float y_end, const std::string &hexcol,
                 float thickness = 1.0f);
void PolylineWindow(const std::vector<Vec2> &points, const std::string &hexcol,
                    float thickness = 1.0f);
void PolylineScreen(const std::vector<Vec2> &points, const std::string &hexcol,
                    float thickness = 1.0f);
void CrossWindow(Vec2 center, float size, const std::string &hexcol,
                 float thickness = 1.0f);
void CrossScreen(Vec2 center, float size, const std::string &hexcol,
                 float thickness = 1.0f);
void RectWindow(Vec2 pos, Vec2 size, const std::string &hexcol);
void RectScreen(Vec2 pos, Vec2 size, const std::string &hexcol);

// Rectangles (outline)
void RectOutlineWindow(Vec2 pos, Vec2 size, const std::string &hexcol,
                       float thickness = 1.0f);
void RectOutlineScreen(Vec2 pos, Vec2 size, const std::string &hexcol,
                       float thickness = 1.0f);

// Squares
void SquareWindow(Vec2 pos, float size, const std::string &hexcol);
void SquareScreen(Vec2 pos, float size, const std::string &hexcol);
void QuadWindow(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const std::string &hexcol);
void QuadScreen(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const std::string &hexcol);

// Texts
void TextWindow(const std::string &text, Vec2 pos, float size,
                const std::string &hexcol);
void TextScreen(const std::string &text, Vec2 pos, float size,
                const std::string &hexcol);
} // namespace Draw
} // namespace Cherry
