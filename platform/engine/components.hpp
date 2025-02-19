#pragma once

#include "base.hpp"

#include "../../src/layer.hpp"
#include "../../src/core/color.hpp"

#include <mutex>
#include <string>
#include <unordered_map>

#ifndef CHERRY_COMPONENT_H
#define CHERRY_COMPONENT_H

namespace Cherry
{
    class Component
    {
    public:
        Component(const Identifier &id);
        Component();

        // Properties
        std::string SetProperty(const std::string &key, const std::string &val);
        std::string GetProperty(const std::string &key);
        void        ClearProperty(const std::string &key);
        void        RefreshContextProperties(); // Get active context properties

        // Data
        std::string SetData(const std::string &key, const std::string &val);
        std::string GetData(const std::string &key);
        void        ClearData(const std::string &key);

        // Identifier
        void        SetIdentifier(Identifier id);
        Identifier  GetIdentifier() const;
        bool        NeedRefreshing();


        virtual void Render() {};

    private:
        Identifier m_Identifier;
        bool m_IsPropsChanged = false;
        std::unordered_map<std::string, std::string> m_ContextProperties;   // Cpy of props registered in the cherry context (from permanent or ontime), first choice
        std::unordered_map<std::string, std::string> m_Properties;          // Cpy of props registered in the component, second choice
        std::unordered_map<std::string, std::string> m_ContextData;
        std::unordered_map<std::string, std::string> m_Data;
    };
}

#endif // CHERRY_COMPONENT_H