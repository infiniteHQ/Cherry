#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_SIMPLE_INPUTS
#define UIKIT_V1_SIMPLE_INPUTS

namespace UIKit
{

    class SimpleStringInput : public Component
    {
    public:
        SimpleStringInput(
            const std::string &id,
            const std::shared_ptr<std::string> &value,
            const std::string &label = "String")
            : Component(id),
              m_Label(label),
              m_Value(value)
        {
        }

        void Render(const std::string &duplication_name)
        {
            if (m_Value)
            {
                char buffer[256];
                std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));
                ImGui::InputText(m_Label.c_str(), buffer, sizeof(buffer));
                *m_Value = std::string(buffer);
            }
            else
            {
                ImGui::Text("INVALID VALUE");
            }
        }

        std::string GetData(const std::string &data_type) override
        {
            if (data_type == "value")
            {
                return *m_Value.get();
            }

            return "none";
        }

    private:
        std::shared_ptr<std::string> m_Value;
        std::string m_Label;
    };

}

#endif // UIKIT_V1_SIMPLE_INPUTS
