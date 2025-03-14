#pragma once

#include "base.hpp"
#include <mutex>

#ifndef CHERRY_IDENTIFIER
#define CHERRY_IDENTIFIER

namespace Cherry
{
    class Component;

    class Identifier
    {
    public:
        Identifier() : m_IdentifierName(generate_id()) {}
        explicit Identifier(const std::string &id) : m_IdentifierName(id) {}

        explicit Identifier(std::vector<std::shared_ptr<Component>> *components_, const std::string &id) 
            : m_IdentifierName(id), m_ComponentArrayPtr(components_) {}

        Identifier(const Identifier &other) = default;
        Identifier& operator=(const Identifier &other) = default;

        [[nodiscard]] const std::string& string() const { return m_IdentifierName; }
        [[nodiscard]] std::vector<std::shared_ptr<Component>> *component_array_ptr() const { return m_ComponentArrayPtr; }
        [[nodiscard]] const std::string& component_group() const { return m_ComponentGroup; }

        void set(const std::string &id) { m_IdentifierName = id; }
        bool operator==(const Identifier &other) const { return m_IdentifierName == other.m_IdentifierName; }

        static void UpgradeIncrementorLevel()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (++incrementor_level > static_cast<int>(level_indices.size()))
                level_indices.push_back(0);
        }

        static void DowngradeIncrementorLevel()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (incrementor_level > 1) --incrementor_level;
        }

        static std::string GetUniqueIndex()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            int level = incrementor_level - 1;
            if (level >= static_cast<int>(level_indices.size()))
                level_indices.push_back(0);
            
            ++level_indices[level];
            return generate_id();
        }

        static void ResetUniqueIndex()
        {
            std::lock_guard<std::mutex> lock(mutex_);
            incrementor_level = 1;
            level_indices = {0};
        }

        static std::shared_ptr<Component> GetComponent()
        {
            return nullptr; // TODO
        }

    private:
        std::string m_IdentifierName;
        std::vector<std::shared_ptr<Component>> *m_ComponentArrayPtr = nullptr;
        std::string m_ComponentGroup;

        static inline int incrementor_level = 1;
        static inline std::vector<int> level_indices = {0};
        static inline std::mutex mutex_;

        static std::string generate_id()
        {
            std::string id;
            int level = incrementor_level;
            
            for (size_t i = 0; i < level_indices.size() && i < static_cast<size_t>(level); ++i)
            {
                id += std::to_string(level_indices[i]);
                if (i % 2 == 1) id += convert_to_letters(level_indices[i]);
            }
            return id;
        }

        static std::string convert_to_letters(int index)
        {
            std::string result;
            while (index >= 0)
            {
                result.insert(result.begin(), 'a' + (index % 26));
                index = (index / 26) - 1;
            }
            return result;
        }
    };
}

using CherryID = Cherry::Identifier;

#endif // CHERRY_IDENTIFIER
