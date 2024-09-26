#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"

namespace UIKit
{
    class Component
    {
    public:
        Component(const std::string &id) : m_ID(id) {};
        virtual std::string GetData(const std::string &data_type) { return "not_implemented"; };
        std::string m_ID;
    };
}
