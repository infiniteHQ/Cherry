#pragma once

#include "../../lib/glm/glm/glm.hpp"
#include "../../lib/imgui/backends/imgui_impl_vulkan.h"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_internal.h"
#include "../../lib/json/single_include/nlohmann/json.hpp"
#include "../../lib/stb-image/stb_image.h"

namespace Cherry {
  static std::string HslaHex(float h, float s, float l, float a = 1.0f) {
    h = fmodf(h, 360.f);
    if (h < 0)
      h += 360.f;
    s = std::clamp(s, 0.f, 1.f);
    l = std::clamp(l, 0.f, 1.f);
    a = std::clamp(a, 0.f, 1.f);
    float c = (1.f - fabsf(2.f * l - 1.f)) * s;
    float x = c * (1.f - fabsf(fmodf(h / 60.f, 2.f) - 1.f));
    float m = l - c * 0.5f;
    float r = 0, g = 0, b = 0;
    int seg = (int)(h / 60.f) % 6;
    switch (seg) {
      case 0:
        r = c;
        g = x;
        b = 0;
        break;
      case 1:
        r = x;
        g = c;
        b = 0;
        break;
      case 2:
        r = 0;
        g = c;
        b = x;
        break;
      case 3:
        r = 0;
        g = x;
        b = c;
        break;
      case 4:
        r = x;
        g = 0;
        b = c;
        break;
      case 5:
        r = c;
        g = 0;
        b = x;
        break;
    }
    char buf[10];
    snprintf(
        buf,
        sizeof(buf),
        "#%02X%02X%02X%02X",
        (int)((r + m) * 255.f),
        (int)((g + m) * 255.f),
        (int)((b + m) * 255.f),
        (int)(a * 255.f));
    return buf;
  }

  static uint32_t ParseHexSafe(std::string hex) {
    if (hex.empty())
      return 0x000000FF;

    if (hex[0] == '#') {
      hex.erase(0, 1);
    }

    if (hex.size() == 6) {
      hex += "FF";
    } else if (hex.size() != 8) {
      return 0x000000FF;
    }

    try {
      size_t pos;
      uint32_t value = std::stoul(hex, &pos, 16);

      if (pos != hex.size())
        return 0x000000FF;

      return value;
    } catch (...) {
      return 0x000000FF;
    }
  }

  static ImU32 HexToImU32(std::string hex) {
    uint32_t v = ParseHexSafe(hex);
    return IM_COL32((v >> 24) & 0xFF, (v >> 16) & 0xFF, (v >> 8) & 0xFF, v & 0xFF);
  }

  static ImVec4 HexToRGBA(std::string hex) {
    uint32_t v = ParseHexSafe(hex);
    constexpr float inv255 = 1.0f / 255.0f;
    return ImVec4(((v >> 24) & 0xFF) * inv255, ((v >> 16) & 0xFF) * inv255, ((v >> 8) & 0xFF) * inv255, (v & 0xFF) * inv255);
  }

  static ImColor HexToImColor(std::string hex) {
    return ImColor(HexToRGBA(hex));
  }

  static std::string ImU32ToHex(ImU32 color) {
    std::array<char, 10> hexStr;
    std::snprintf(
        hexStr.data(),
        hexStr.size(),
        "#%02X%02X%02X%02X",
        (color >> IM_COL32_R_SHIFT) & 0xFF,  // R
        (color >> IM_COL32_G_SHIFT) & 0xFF,  // G
        (color >> IM_COL32_B_SHIFT) & 0xFF,  // B
        (color >> IM_COL32_A_SHIFT) & 0xFF   // A
    );
    return std::string(hexStr.data());
  }
}  // namespace Cherry