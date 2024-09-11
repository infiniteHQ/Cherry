#pragma once

#include "../layer.hpp"

#include <string>

namespace UIKit
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
}