#include "components.hpp"
#include "app.hpp"

namespace Cherry
{
    Component::Component(const Cherry::Identifier &id) : m_Identifier(id) {};
    Component::Component() {};

    std::string Component::SetData(const std::string &key, const std::string &val)
    {
        m_Data[key] = val;
        return val;
    }

    std::string Component::GetData(const std::string &key)
    {
        auto it = m_Data.find(key);
        if (it != m_Data.end())
        {
            return it->second;
        }
        return "undefined";
    }

    void Component::ClearData(const std::string &key)
    {
        m_Data.erase(key);
    }

    std::string Component::SetProperty(const std::string &key, const std::string &val)
    {
        m_Properties[key] = val;
        m_IsPropsChanged = true;
        return val;
    }

    void Component::RenderWrapper()
    {
        // If render disable flag enabled, dont render and consume properties
        // button->RefreshContextProperties();

        this->Render();
    }

    std::string Component::GetProperty(const std::string &key)
    {
        if (!m_IsPropsChanged)
        {
            auto cacheIt = m_CachedProperties.find(key);
            if (cacheIt != m_CachedProperties.end())
            {
                return cacheIt->second;
            }
        }

        std::string value;

        auto onetimeMap = Application::Get().GetOneTimeProperties();
        auto onetimeIt = onetimeMap.find(key);
        if (onetimeIt != onetimeMap.end())
        {
            value = onetimeIt->second;
            Application::Get().RemoveOneTimeProperty(key);
        }
        else
        {
            auto ctxIt = m_ContextProperties.find(key);
            if (ctxIt != m_ContextProperties.end())
            {
                value = ctxIt->second;
            }
            else
            {
                auto it = m_Properties.find(key);
                if (it != m_Properties.end())
                {
                    value = it->second;
                }
                else
                {
                    value = "undefined";
                }
            }
        }

        if (!m_IsPropsChanged)
        {
            m_CachedProperties[key] = value;
        }

        return value;
    }

    void Component::ClearProperty(const std::string &key)
    {
        m_IsPropsChanged = true;
        m_Properties.erase(key);
    }

    void Component::RefreshContextProperties()
    {
        m_ContextProperties.clear();

        // Insert all one time props
        for (const auto &pair : Application::Get().m_OnTimeProperties)
        {
            m_ContextProperties.emplace(pair);
        }
        Application::Get().m_OnTimeProperties.clear(); // One time properties consumed.

        // Insert all permanent props
        for (const auto &pair : Application::Get().m_PermanentProperties)
        {
            m_ContextProperties.emplace(pair);
        }
    }

    void Component::SetIdentifier(const Cherry::Identifier &id)
    {
        m_Identifier = id;
    }

    const Identifier &Component::GetIdentifier() const
    {
        return m_Identifier;
    }

    bool Component::NeedRefreshing()
    {
        return m_IsPropsChanged;
    }

    void Component::Refreshed()
    {
        m_IsPropsChanged = false;
    }

}
