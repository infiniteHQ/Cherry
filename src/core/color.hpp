#pragma once

#include "../layer.hpp"

#include <string>

namespace Cherry
{
    static ImVec4 HexToRGBA(const std::string &hex)
    {
        unsigned int hexValue;
        std::stringstream ss;
        ss << std::hex << hex.substr(1);
        ss >> hexValue;

        float r = ((hexValue >> 24) & 0xFF) / 255.0f;
        float g = ((hexValue >> 16) & 0xFF) / 255.0f;
        float b = ((hexValue >> 8) & 0xFF) / 255.0f;
        float a = (hexValue & 0xFF) / 255.0f;

        return ImVec4(r, g, b, a);
    }

    static ImU32 HexToImU32(const std::string &hex)
    {
        unsigned int hexValue;
        std::stringstream ss;
        ss << std::hex << hex.substr(1);
        ss >> hexValue;

        unsigned int r = (hexValue >> 24) & 0xFF;
        unsigned int g = (hexValue >> 16) & 0xFF;
        unsigned int b = (hexValue >> 8) & 0xFF;
        unsigned int a = hexValue & 0xFF;

        return IM_COL32(r, g, b, a);
    }

    static std::string ImU32ToHex(ImU32 color)
    {
        unsigned int r = (color >> IM_COL32_R_SHIFT) & 0xFF;
        unsigned int g = (color >> IM_COL32_G_SHIFT) & 0xFF;
        unsigned int b = (color >> IM_COL32_B_SHIFT) & 0xFF;
        unsigned int a = (color >> IM_COL32_A_SHIFT) & 0xFF;

        std::stringstream ss;
        ss << "#" << std::hex << std::setw(2) << std::setfill('0') << r
           << std::setw(2) << std::setfill('0') << g
           << std::setw(2) << std::setfill('0') << b
           << std::setw(2) << std::setfill('0') << a;

        return ss.str();
    }
}