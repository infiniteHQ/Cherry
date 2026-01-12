#pragma once

#include "../layer.hpp"

namespace Cherry {
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
  return IM_COL32((v >> 24) & 0xFF, (v >> 16) & 0xFF, (v >> 8) & 0xFF,
                  v & 0xFF);
}

static ImVec4 HexToRGBA(std::string hex) {
  uint32_t v = ParseHexSafe(hex);
  constexpr float inv255 = 1.0f / 255.0f;
  return ImVec4(((v >> 24) & 0xFF) * inv255, ((v >> 16) & 0xFF) * inv255,
                ((v >> 8) & 0xFF) * inv255, (v & 0xFF) * inv255);
}

static ImColor HexToImColor(std::string hex) { return ImColor(HexToRGBA(hex)); }

static std::string ImU32ToHex(ImU32 color) {
  std::array<char, 10> hexStr;
  std::snprintf(hexStr.data(), hexStr.size(), "#%02X%02X%02X%02X",
                (color >> IM_COL32_R_SHIFT) & 0xFF, // R
                (color >> IM_COL32_G_SHIFT) & 0xFF, // G
                (color >> IM_COL32_B_SHIFT) & 0xFF, // B
                (color >> IM_COL32_A_SHIFT) & 0xFF  // A
  );
  return std::string(hexStr.data());
}
} // namespace Cherry