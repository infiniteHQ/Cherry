#pragma once

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"

#include <string>
#include <unordered_map>

#ifndef CHERRY_COMPONENT_H
#define CHERRY_COMPONENT_H

namespace Cherry
{
    class Component
    {
    public:
        Component(const std::string &id);

        // OBSOLETE
        virtual std::string GetData(const std::string &data_type);
        std::string SetProp(const std::string &key, const std::string &val);
        std::string GetProp(const std::string &key);
        void ClearProp(const std::string &key);
        void SetID(const std::string &id);
        std::string GetID();
        bool NeedRefreshing();

    private:
        std::string m_ID;
        bool m_IsPropsChanged = false;
        std::unordered_map<std::string, std::string> m_Properties;
    };
}

#endif // CHERRY_COMPONENT_H