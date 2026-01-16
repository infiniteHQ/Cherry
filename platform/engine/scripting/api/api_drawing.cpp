
#include "../../drawing/drawing.hpp"
#include "../../imgui/wrappers/wrappers.hpp"
#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING
#include "../lua_helpers.hpp"

namespace Cherry {
namespace Script {

LUA_FUNC(DrawRect) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::RectWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color);

  return 0;
}

LUA_FUNC(DrawRectFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::RectScreen(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color);

  return 0;
}

LUA_FUNC(DrawRectOutline) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::RectOutlineWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color,
                                  thickness);

  return 0;
}

LUA_FUNC(DrawRectOutlineFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::RectOutlineScreen(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color,
                                  thickness);

  return 0;
}

LUA_FUNC(DrawText) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  std::string text = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::TextWindow(text, Cherry::Vec2(x, y), size, color);

  return 0;
}

LUA_FUNC(DrawTextFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  std::string text = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::TextScreen(text, Cherry::Vec2(x, y), size, color);

  return 0;
}

LUA_FUNC(DrawLine) {
  float sx = lua_getfloat(L, 1);
  float sy = lua_getfloat(L, 2);
  float ex = lua_getfloat(L, 3);
  float ey = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::LineWindow(Cherry::Vec2(sx, sy), Cherry::Vec2(ex, ey), color,
                           thickness);

  return 0;
}

LUA_FUNC(DrawLineFixed) {
  float sx = lua_getfloat(L, 1);
  float sy = lua_getfloat(L, 2);
  float ex = lua_getfloat(L, 3);
  float ey = lua_getfloat(L, 4);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::LineScreen(Cherry::Vec2(sx, sy), Cherry::Vec2(ex, ey), color,
                           thickness);

  return 0;
}

LUA_FUNC(DrawLineHorizontal) {
  float y = lua_getfloat(L, 1);
  float sx = lua_getfloat(L, 2);
  float ex = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = (lua_gettop(L) >= 5) ? lua_getfloat(L, 5) : 1.0f;

  Cherry::Draw::LineHWindow(y, sx, ex, color, thickness);
  return 0;
}

LUA_FUNC(DrawLineHorizontalFixed) {
  float y = lua_getfloat(L, 1);
  float sx = lua_getfloat(L, 2);
  float ex = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = (lua_gettop(L) >= 5) ? lua_getfloat(L, 5) : 1.0f;

  Cherry::Draw::LineHScreen(y, sx, ex, color, thickness);
  return 0;
}

LUA_FUNC(DrawLineVertical) {
  float x = lua_getfloat(L, 1);
  float sy = lua_getfloat(L, 2);
  float ey = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = (lua_gettop(L) >= 5) ? lua_getfloat(L, 5) : 1.0f;

  Cherry::Draw::LineVWindow(x, sy, ey, color, thickness);
  return 0;
}

LUA_FUNC(DrawLineVerticalFixed) {
  float x = lua_getfloat(L, 1);
  float sy = lua_getfloat(L, 2);
  float ey = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = (lua_gettop(L) >= 5) ? lua_getfloat(L, 5) : 1.0f;

  Cherry::Draw::LineVScreen(x, sy, ey, color, thickness);
  return 0;
}
LUA_FUNC(DrawPolyline) {
  if (!lua_istable(L, 1)) {
    return 0;
  }

  std::vector<Cherry::Vec2> points;
  int n = lua_rawlen(L, 1);

  for (int i = 1; i <= n; i++) {
    lua_rawgeti(L, 1, i);

    if (lua_istable(L, -1)) {
      lua_rawgeti(L, -1, 1); // x
      float x = lua_tonumber(L, -1);
      lua_pop(L, 1);

      lua_rawgeti(L, -1, 2); // y
      float y = lua_tonumber(L, -1);
      lua_pop(L, 1);

      points.push_back(Cherry::Vec2(x, y));
    }
    lua_pop(L, 1);
  }

  std::string color = static_cast<std::string>(lua_tostring(L, 2));
  float thickness = (float)lua_tonumber(L, 3);
  Cherry::Draw::PolylineWindow(points, color, thickness);

  return 0;
}

LUA_FUNC(DrawPolylineFixed) {
  if (!lua_istable(L, 1)) {
    return 0;
  }

  std::vector<Cherry::Vec2> points;
  int n = lua_rawlen(L, 1);

  for (int i = 1; i <= n; i++) {
    lua_rawgeti(L, 1, i);

    if (lua_istable(L, -1)) {
      lua_rawgeti(L, -1, 1); // x
      float x = lua_tonumber(L, -1);
      lua_pop(L, 1);

      lua_rawgeti(L, -1, 2); // y
      float y = lua_tonumber(L, -1);
      lua_pop(L, 1);

      points.push_back(Cherry::Vec2(x, y));
    }
    lua_pop(L, 1);
  }

  std::string color = static_cast<std::string>(lua_tostring(L, 2));
  float thickness = (float)lua_tonumber(L, 3);
  Cherry::Draw::PolylineScreen(points, color, thickness);

  return 0;
}

LUA_FUNC(DrawCross) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = (lua_gettop(L) >= 5) ? lua_getfloat(L, 5) : 1.0f;

  Cherry::Draw::CrossWindow(Cherry::Vec2(x, y), size, color, thickness);

  return 0;
}

LUA_FUNC(DrawCrossFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = (lua_gettop(L) >= 5) ? lua_getfloat(L, 5) : 1.0f;

  Cherry::Draw::CrossScreen(Cherry::Vec2(x, y), size, color, thickness);

  return 0;
}
LUA_FUNC(DrawSquare) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));

  Cherry::Draw::SquareWindow(Cherry::Vec2(x, y), size, color);

  return 0;
}

LUA_FUNC(DrawSquareFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));

  Cherry::Draw::SquareScreen(Cherry::Vec2(x, y), size, color);

  return 0;
}

LUA_FUNC(DrawQuad) {
  float x0 = lua_getfloat(L, 1);
  float y0 = lua_getfloat(L, 2);

  float x1 = lua_getfloat(L, 3);
  float y1 = lua_getfloat(L, 4);

  float x2 = lua_getfloat(L, 5);
  float y2 = lua_getfloat(L, 6);

  float x3 = lua_getfloat(L, 7);
  float y3 = lua_getfloat(L, 8);

  std::string color = static_cast<std::string>(lua_getstring(L, 9));

  Cherry::Draw::QuadWindow(Cherry::Vec2(x0, y0), Cherry::Vec2(x1, y1),
                           Cherry::Vec2(x2, y2), Cherry::Vec2(x3, y3), color);

  return 0;
}

LUA_FUNC(DrawQuadFixed) {
  float x0 = lua_getfloat(L, 1);
  float y0 = lua_getfloat(L, 2);

  float x1 = lua_getfloat(L, 3);
  float y1 = lua_getfloat(L, 4);

  float x2 = lua_getfloat(L, 5);
  float y2 = lua_getfloat(L, 6);

  float x3 = lua_getfloat(L, 7);
  float y3 = lua_getfloat(L, 8);

  std::string color = static_cast<std::string>(lua_getstring(L, 9));

  Cherry::Draw::QuadScreen(Cherry::Vec2(x0, y0), Cherry::Vec2(x1, y1),
                           Cherry::Vec2(x2, y2), Cherry::Vec2(x3, y3), color);

  return 0;
}
LUA_FUNC(DrawCircle) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  int segments = (int)lua_getfloat(L, 5); // 0 = auto

  Cherry::Draw::CircleWindow(Cherry::Vec2(x, y), radius, color, segments);
  return 0;
}

LUA_FUNC(DrawCircleFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  int segments = (int)lua_getfloat(L, 5);

  Cherry::Draw::CircleScreen(Cherry::Vec2(x, y), radius, color, segments);
  return 0;
}

LUA_FUNC(DrawCircleOutline) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = lua_getfloat(L, 5);
  int segments = (int)lua_getfloat(L, 6);

  Cherry::Draw::CircleOutlineWindow(Cherry::Vec2(x, y), radius, color,
                                    thickness, segments);
  return 0;
}

LUA_FUNC(DrawCircleOutlineFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 4));
  float thickness = lua_getfloat(L, 5);
  int segments = (int)lua_getfloat(L, 6);

  Cherry::Draw::CircleOutlineScreen(Cherry::Vec2(x, y), radius, color,
                                    thickness, segments);
  return 0;
}

LUA_FUNC(DrawTriangle) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  std::string color = static_cast<std::string>(lua_getstring(L, 7));

  Cherry::Draw::TriangleWindow(p1, p2, p3, color);
  return 0;
}

LUA_FUNC(DrawTriangleFixed) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  std::string color = static_cast<std::string>(lua_getstring(L, 7));

  Cherry::Draw::TriangleScreen(p1, p2, p3, color);
  return 0;
}

LUA_FUNC(DrawRectRounded) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  float rounding = lua_getfloat(L, 5);
  std::string color = static_cast<std::string>(lua_getstring(L, 6));

  Cherry::Draw::RectRoundedWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h),
                                  rounding, color);
  return 0;
}

LUA_FUNC(DrawRectOutlineRounded) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  float rounding = lua_getfloat(L, 5);
  std::string color = static_cast<std::string>(lua_getstring(L, 6));
  float thickness = lua_getfloat(L, 7);

  Cherry::Draw::RectOutlineRoundedWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h),
                                         rounding, color, thickness);
  return 0;
}

LUA_FUNC(DrawRectOutlineRoundedFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  float rounding = lua_getfloat(L, 5);
  std::string color = static_cast<std::string>(lua_getstring(L, 6));
  float thickness = lua_getfloat(L, 7);

  Cherry::Draw::RectOutlineRoundedWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h),
                                         rounding, color, thickness);
  return 0;
}

LUA_FUNC(DrawRectRoundedFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  float rounding = lua_getfloat(L, 5);
  std::string color = static_cast<std::string>(lua_getstring(L, 6));

  Cherry::Draw::RectRoundedScreen(Cherry::Vec2(x, y), Cherry::Vec2(w, h),
                                  rounding, color);
  return 0;
}

LUA_FUNC(DrawImageRounded) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  float rounding = lua_getfloat(L, 5);
  std::string path = static_cast<std::string>(lua_getstring(L, 6));
  std::string color = (lua_gettop(L) >= 7)
                          ? static_cast<std::string>(lua_getstring(L, 7))
                          : "#FFFFFFFF";

  Cherry::Draw::ImageRoundedWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h), path,
                                   rounding, color);
  return 0;
}

LUA_FUNC(DrawImageRoundedFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  float rounding = lua_getfloat(L, 5);
  std::string path = static_cast<std::string>(lua_getstring(L, 6));
  std::string color = (lua_gettop(L) >= 7)
                          ? static_cast<std::string>(lua_getstring(L, 7))
                          : "#FFFFFFFF";

  Cherry::Draw::ImageRoundedScreen(Cherry::Vec2(x, y), Cherry::Vec2(w, h), path,
                                   rounding, color);
  return 0;
}

LUA_FUNC(DrawImage) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string path = static_cast<std::string>(lua_getstring(L, 5));

  float uv_min_x = (lua_gettop(L) >= 6) ? lua_getfloat(L, 6) : 0.0f;
  float uv_min_y = (lua_gettop(L) >= 7) ? lua_getfloat(L, 7) : 0.0f;
  float uv_max_x = (lua_gettop(L) >= 8) ? lua_getfloat(L, 8) : 1.0f;
  float uv_max_y = (lua_gettop(L) >= 9) ? lua_getfloat(L, 9) : 1.0f;

  std::string color = (lua_gettop(L) >= 10)
                          ? static_cast<std::string>(lua_getstring(L, 10))
                          : "#FFFFFFFF";

  Cherry::Draw::ImageWindow({x, y}, {w, h}, path, {uv_min_x, uv_min_y},
                            {uv_max_x, uv_max_y}, color);
  return 0;
}

LUA_FUNC(DrawImageFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string path = static_cast<std::string>(lua_getstring(L, 5));

  float uv_min_x = (lua_gettop(L) >= 6) ? lua_getfloat(L, 6) : 0.0f;
  float uv_min_y = (lua_gettop(L) >= 7) ? lua_getfloat(L, 7) : 0.0f;
  float uv_max_x = (lua_gettop(L) >= 8) ? lua_getfloat(L, 8) : 1.0f;
  float uv_max_y = (lua_gettop(L) >= 9) ? lua_getfloat(L, 9) : 1.0f;

  std::string color = (lua_gettop(L) >= 10)
                          ? static_cast<std::string>(lua_getstring(L, 10))
                          : "#FFFFFFFF";

  Cherry::Draw::ImageScreen({x, y}, {w, h}, path, {uv_min_x, uv_min_y},
                            {uv_max_x, uv_max_y}, color);
  return 0;
}

LUA_FUNC(DrawImageQuad) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  Cherry::Vec2 p4(lua_getfloat(L, 7), lua_getfloat(L, 8));
  std::string path = static_cast<std::string>(lua_getstring(L, 9));
  std::string color = (lua_gettop(L) >= 10)
                          ? static_cast<std::string>(lua_getstring(L, 10))
                          : "#FFFFFFFF";

  Cherry::Draw::ImageQuadWindow(p1, p2, p3, p4, path, color);
  return 0;
}

LUA_FUNC(DrawImageQuadFixed) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  Cherry::Vec2 p4(lua_getfloat(L, 7), lua_getfloat(L, 8));
  std::string path = static_cast<std::string>(lua_getstring(L, 9));
  std::string color = (lua_gettop(L) >= 10)
                          ? static_cast<std::string>(lua_getstring(L, 10))
                          : "#FFFFFFFF";

  Cherry::Draw::ImageQuadScreen(p1, p2, p3, p4, path, color);
  return 0;
}

LUA_FUNC(DrawRectGradient) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string c1 = lua_getstring(L, 5);
  std::string c2 = lua_getstring(L, 6);
  bool vertical = (lua_gettop(L) >= 7) ? lua_toboolean(L, 7) : true;
  int steps = (lua_gettop(L) >= 8) ? (int)lua_getfloat(L, 8) : 32;

  Cherry::Draw::RectGradientWindow({x, y}, {w, h}, c1, c2, vertical, steps);
  return 0;
}

LUA_FUNC(DrawRectGradientFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string c1 = lua_getstring(L, 5);
  std::string c2 = lua_getstring(L, 6);
  bool vertical = (lua_gettop(L) >= 7) ? lua_toboolean(L, 7) : true;
  int steps = (lua_gettop(L) >= 8) ? (int)lua_getfloat(L, 8) : 32;

  Cherry::Draw::RectGradientScreen({x, y}, {w, h}, c1, c2, vertical, steps);
  return 0;
}

LUA_FUNC(DrawRectOutlineGradient) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string c1 = lua_getstring(L, 5);
  std::string c2 = lua_getstring(L, 6);
  float thickness = (lua_gettop(L) >= 7) ? lua_getfloat(L, 7) : 1.0f;

  Cherry::Draw::RectOutlineGradientWindow({x, y}, {w, h}, c1, c2, thickness);
  return 0;
}

LUA_FUNC(DrawRectOutlineGradientFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  std::string c1 = lua_getstring(L, 5);
  std::string c2 = lua_getstring(L, 6);
  float thickness = (lua_gettop(L) >= 7) ? lua_getfloat(L, 7) : 1.0f;

  Cherry::Draw::RectOutlineGradientScreen({x, y}, {w, h}, c1, c2, thickness);
  return 0;
}

LUA_FUNC(DrawCircleGradient) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string c1 = lua_getstring(L, 4);
  std::string c2 = lua_getstring(L, 5);
  int steps = (lua_gettop(L) >= 6) ? (int)lua_getfloat(L, 6) : 16;

  Cherry::Draw::CircleGradientWindow({x, y}, radius, c1, c2, steps);
  return 0;
}

LUA_FUNC(DrawCircleGradientFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string c1 = lua_getstring(L, 4);
  std::string c2 = lua_getstring(L, 5);
  int steps = (lua_gettop(L) >= 6) ? (int)lua_getfloat(L, 6) : 16;

  Cherry::Draw::CircleGradientScreen({x, y}, radius, c1, c2, steps);
  return 0;
}

LUA_FUNC(DrawCircleOutlineGradient) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string c1 = lua_getstring(L, 4);
  std::string c2 = lua_getstring(L, 5);
  float thickness = (lua_gettop(L) >= 6) ? lua_getfloat(L, 6) : 1.0f;

  Cherry::Draw::CircleOutlineGradientWindow({x, y}, radius, c1, c2, thickness);
  return 0;
}

LUA_FUNC(DrawCircleOutlineGradientFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float radius = lua_getfloat(L, 3);
  std::string c1 = lua_getstring(L, 4);
  std::string c2 = lua_getstring(L, 5);
  float thickness = (lua_gettop(L) >= 6) ? lua_getfloat(L, 6) : 1.0f;

  Cherry::Draw::CircleOutlineGradientScreen({x, y}, radius, c1, c2, thickness);
  return 0;
}
LUA_FUNC(DrawTriangleGradient) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  std::string c1 = lua_getstring(L, 7);
  std::string c2 = lua_getstring(L, 8);
  std::string c3 = lua_getstring(L, 9);

  Cherry::Draw::TriangleGradientWindow(p1, p2, p3, c1, c2, c3);
  return 0;
}

LUA_FUNC(DrawTriangleGradientFixed) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  std::string c1 = lua_getstring(L, 7);
  std::string c2 = lua_getstring(L, 8);
  std::string c3 = lua_getstring(L, 9);

  Cherry::Draw::TriangleGradientScreen(p1, p2, p3, c1, c2, c3);
  return 0;
}

LUA_FUNC(DrawTriangleOutlineGradient) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  std::string c1 = lua_getstring(L, 7);
  std::string c2 = lua_getstring(L, 8);
  float thickness = (lua_gettop(L) >= 9) ? lua_getfloat(L, 9) : 1.0f;

  Cherry::Draw::TriangleOutlineGradientWindow(p1, p2, p3, c1, c2, thickness);
  return 0;
}

LUA_FUNC(DrawTriangleOutlineGradientFixed) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  std::string c1 = lua_getstring(L, 7);
  std::string c2 = lua_getstring(L, 8);
  float thickness = (lua_gettop(L) >= 9) ? lua_getfloat(L, 9) : 1.0f;

  Cherry::Draw::TriangleOutlineGradientScreen(p1, p2, p3, c1, c2, thickness);
  return 0;
}

LUA_FUNC(DrawQuadGradient) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  Cherry::Vec2 p4(lua_getfloat(L, 7), lua_getfloat(L, 8));
  std::string c1 = lua_getstring(L, 9);
  std::string c2 = lua_getstring(L, 10);
  std::string c3 = lua_getstring(L, 11);
  std::string c4 = lua_getstring(L, 12);

  Cherry::Draw::QuadGradientWindow(p1, p2, p3, p4, c1, c2, c3, c4);
  return 0;
}

LUA_FUNC(DrawQuadGradientFixed) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  Cherry::Vec2 p4(lua_getfloat(L, 7), lua_getfloat(L, 8));
  std::string c1 = lua_getstring(L, 9);
  std::string c2 = lua_getstring(L, 10);
  std::string c3 = lua_getstring(L, 11);
  std::string c4 = lua_getstring(L, 12);

  Cherry::Draw::QuadGradientScreen(p1, p2, p3, p4, c1, c2, c3, c4);
  return 0;
}

LUA_FUNC(DrawQuadOutlineGradient) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  Cherry::Vec2 p4(lua_getfloat(L, 7), lua_getfloat(L, 8));
  std::string c1 = lua_getstring(L, 9);
  std::string c2 = lua_getstring(L, 10);
  float thickness = (lua_gettop(L) >= 11) ? lua_getfloat(L, 11) : 1.0f;

  Cherry::Draw::QuadOutlineGradientWindow(p1, p2, p3, p4, c1, c2, thickness);
  return 0;
}

LUA_FUNC(DrawQuadOutlineGradientFixed) {
  Cherry::Vec2 p1(lua_getfloat(L, 1), lua_getfloat(L, 2));
  Cherry::Vec2 p2(lua_getfloat(L, 3), lua_getfloat(L, 4));
  Cherry::Vec2 p3(lua_getfloat(L, 5), lua_getfloat(L, 6));
  Cherry::Vec2 p4(lua_getfloat(L, 7), lua_getfloat(L, 8));
  std::string c1 = lua_getstring(L, 9);
  std::string c2 = lua_getstring(L, 10);
  float thickness = (lua_gettop(L) >= 11) ? lua_getfloat(L, 11) : 1.0f;

  Cherry::Draw::QuadOutlineGradientScreen(p1, p2, p3, p4, c1, c2, thickness);
  return 0;
}

LUA_FUNC(BeginRectMask) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  bool intersect = (lua_gettop(L) >= 5) ? lua_toboolean(L, 5) : true;

  Cherry::Draw::BeginRectMask({x, y}, {w, h}, intersect);
  return 0;
}

LUA_FUNC(EndRectMask) {
  Cherry::Draw::EndRectMask();
  return 0;
}

LUA_FUNC(BeginRectMaskFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float w = lua_getfloat(L, 3);
  float h = lua_getfloat(L, 4);
  bool intersect = (lua_gettop(L) >= 5) ? lua_toboolean(L, 5) : true;

  Cherry::Draw::BeginRectMaskFixed({x, y}, {w, h}, intersect);
  return 0;
}

LUA_FUNC(EndRectMaskFixed) {
  Cherry::Draw::EndRectMaskFixed();
  return 0;
}

void RegisterDrawingAPI(lua_State *L) {
  LUA_REGISTER(L, -1, DrawRect);
  LUA_REGISTER(L, -1, DrawRectFixed);
  LUA_REGISTER(L, -1, DrawRectOutline);
  LUA_REGISTER(L, -1, DrawRectOutlineFixed);
  LUA_REGISTER(L, -1, DrawRectOutlineRounded);
  LUA_REGISTER(L, -1, DrawRectOutlineRoundedFixed);
  LUA_REGISTER(L, -1, DrawText);
  LUA_REGISTER(L, -1, DrawTextFixed);
  LUA_REGISTER(L, -1, DrawLine);
  LUA_REGISTER(L, -1, DrawLineFixed);
  LUA_REGISTER(L, -1, DrawLineHorizontal);
  LUA_REGISTER(L, -1, DrawLineHorizontalFixed);
  LUA_REGISTER(L, -1, DrawLineVertical);
  LUA_REGISTER(L, -1, DrawLineVerticalFixed);
  LUA_REGISTER(L, -1, DrawPolyline);
  LUA_REGISTER(L, -1, DrawPolylineFixed);
  LUA_REGISTER(L, -1, DrawCross);
  LUA_REGISTER(L, -1, DrawCrossFixed);
  LUA_REGISTER(L, -1, DrawSquare);
  LUA_REGISTER(L, -1, DrawSquareFixed);
  LUA_REGISTER(L, -1, DrawQuad);
  LUA_REGISTER(L, -1, DrawQuadFixed);
  LUA_REGISTER(L, -1, DrawCircle);
  LUA_REGISTER(L, -1, DrawCircleFixed);
  LUA_REGISTER(L, -1, DrawCircleOutline);
  LUA_REGISTER(L, -1, DrawCircleOutlineFixed); // Not Fixed
  LUA_REGISTER(L, -1, DrawTriangle);
  LUA_REGISTER(L, -1, DrawTriangleFixed);
  LUA_REGISTER(L, -1, DrawRectRounded);
  LUA_REGISTER(L, -1, DrawRectRoundedFixed);
  LUA_REGISTER(L, -1, DrawImageRounded);
  LUA_REGISTER(L, -1, DrawImageRoundedFixed);
  LUA_REGISTER(L, -1, DrawImage);
  LUA_REGISTER(L, -1, DrawImageFixed);
  LUA_REGISTER(L, -1, DrawImageQuad);
  LUA_REGISTER(L, -1, DrawImageQuadFixed);
  LUA_REGISTER(L, -1, DrawRectGradient);
  LUA_REGISTER(L, -1, DrawRectGradientFixed);
  LUA_REGISTER(L, -1, DrawRectOutlineGradient);
  LUA_REGISTER(L, -1, DrawRectOutlineGradientFixed);
  LUA_REGISTER(L, -1, DrawCircleGradient);
  LUA_REGISTER(L, -1, DrawCircleGradientFixed);
  LUA_REGISTER(L, -1, DrawCircleOutlineGradient);
  LUA_REGISTER(L, -1, DrawCircleOutlineGradientFixed);
  LUA_REGISTER(L, -1, DrawTriangleGradient);
  LUA_REGISTER(L, -1, DrawTriangleGradientFixed);
  LUA_REGISTER(L, -1, DrawTriangleOutlineGradient);
  LUA_REGISTER(L, -1, DrawTriangleOutlineGradientFixed);
  LUA_REGISTER(L, -1, DrawQuadGradient);
  LUA_REGISTER(L, -1, DrawQuadGradientFixed);
  LUA_REGISTER(L, -1, DrawQuadOutlineGradient);
  LUA_REGISTER(L, -1, DrawQuadOutlineGradientFixed);

  LUA_REGISTER(L, -1, BeginRectMask);
  LUA_REGISTER(L, -1, EndRectMask);
  LUA_REGISTER(L, -1, BeginRectMaskFixed);
  LUA_REGISTER(L, -1, EndRectMaskFixed);
}

} // namespace Script
} // namespace Cherry
#endif // CHERRY_ENABLE_SCRIPTING
