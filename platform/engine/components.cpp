#include "components.hpp"

namespace Cherry
{
    Component::Component(const std::string &id) : m_ID(id) {};

    // OBSOLETE
    std::string Component::GetData(const std::string &data_type)
    {
        return "not_implemented";
    };

    std::string Component::SetProp(const std::string &key, const std::string &val)
    {
        m_Properties[key] = val;
        m_IsPropsChanged = true;
        return val;
    }

    std::string Component::GetProp(const std::string &key)
    {
        auto it = m_Properties.find(key);
        if (it != m_Properties.end())
        {
            return it->second;
        }
        return 0;
    }

    void Component::ClearProp(const std::string &key)
    {
        m_IsPropsChanged = true;
        m_Properties.erase(key);
    }

    void Component::SetID(const std::string &id)
    {
        m_ID = id;
    }

    std::string Component::GetID()
    {
        return m_ID;
    }

    bool Component::NeedRefreshing()
    {
        return m_IsPropsChanged;
    }
}
