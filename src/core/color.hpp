#pragma once

#include "../layer.hpp"

namespace Cherry
{
    static ImVec4 HexToRGBA(std::string hex)
    {
        if (hex.size() == 7)
            hex += "FF";

        uint32_t hexValue = std::stoul(hex.substr(1), nullptr, 16);

        constexpr float inv255 = 1.0f / 255.0f;
        return ImVec4(
            ((hexValue >> 24) & 0xFF) * inv255, // R
            ((hexValue >> 16) & 0xFF) * inv255, // G
            ((hexValue >> 8) & 0xFF) * inv255,  // B
            ((hexValue >> 0) & 0xFF) * inv255   // A
        );
    }

    static ImU32 HexToImU32(std::string hex)
    {
        if (hex.size() == 7)
            hex += "FF";

        uint32_t hexValue = std::stoul(hex.substr(1), nullptr, 16);

        return IM_COL32(
            (hexValue >> 24) & 0xFF, // R
            (hexValue >> 16) & 0xFF, // G
            (hexValue >> 8) & 0xFF,  // B
            (hexValue >> 0) & 0xFF   // A
        );
    }

    static std::string ImU32ToHex(ImU32 color)
    {
        std::array<char, 10> hexStr;
        std::snprintf(hexStr.data(), hexStr.size(), "#%02X%02X%02X%02X",
                      (color >> IM_COL32_R_SHIFT) & 0xFF, // R
                      (color >> IM_COL32_G_SHIFT) & 0xFF, // G
                      (color >> IM_COL32_B_SHIFT) & 0xFF, // B
                      (color >> IM_COL32_A_SHIFT) & 0xFF  // A
        );
        return std::string(hexStr.data());
    }
}