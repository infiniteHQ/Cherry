
#include "../../drawing/drawing.hpp"
#include "../../imgui/wrappers/wrappers.hpp"
#include "../lua_helpers.hpp"
#include "../scripting.hpp"

#ifdef CHERRY_ENABLE_SCRIPTING

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
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::LineHWindow(y, sx, ex, color, thickness);

  return 0;
}

LUA_FUNC(DrawLineHorizontalFixed) {
  float y = lua_getfloat(L, 1);
  float sx = lua_getfloat(L, 2);
  float ex = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::LineHScreen(y, sx, ex, color, thickness);

  return 0;
}

LUA_FUNC(DrawLineVertical) {
  float x = lua_getfloat(L, 1);
  float sy = lua_getfloat(L, 2);
  float ey = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::LineVWindow(x, sy, ey, color, thickness);

  return 0;
}

LUA_FUNC(DrawLineVerticalFixed) {
  float x = lua_getfloat(L, 1);
  float sy = lua_getfloat(L, 2);
  float ey = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

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
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::CrossWindow(Cherry::Vec2(x, y), size, color, thickness);

  return 0;
}

LUA_FUNC(DrawCrossFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));
  float thickness = lua_getfloat(L, 6);

  Cherry::Draw::CrossScreen(Cherry::Vec2(x, y), size, color, thickness);

  return 0;
}

LUA_FUNC(DrawSquare) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));

  Cherry::Draw::SquareWindow(Cherry::Vec2(x, y), size, color);

  return 0;
}

LUA_FUNC(DrawSquareFixed) {
  float x = lua_getfloat(L, 1);
  float y = lua_getfloat(L, 2);
  float size = lua_getfloat(L, 3);
  std::string color = static_cast<std::string>(lua_getstring(L, 5));

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

void RegisterDrawingAPI(lua_State *L) {
  LUA_REGISTER(L, -1, DrawRect);
  LUA_REGISTER(L, -1, DrawRectFixed);
  LUA_REGISTER(L, -1, DrawRectOutline);
  LUA_REGISTER(L, -1, DrawRectOutlineFixed);
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
  LUA_REGISTER(L, -1, DrawCircleOutlineFixed);
  LUA_REGISTER(L, -1, DrawTriangle);
  LUA_REGISTER(L, -1, DrawTriangleFixed);
  LUA_REGISTER(L, -1, DrawRectRounded);
  LUA_REGISTER(L, -1, DrawRectRoundedFixed);
}

} // namespace Script
} // namespace Cherry
#endif // CHERRY_ENABLE_SCRIPTING
