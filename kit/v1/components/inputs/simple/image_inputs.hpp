#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_IMAGE_INPUTS
#define UIKIT_V1_IMAGE_INPUTS

namespace UIKit
{

    class ImageStringInput : public Component
    {
    public:
        ImageStringInput(
            const std::string &id,
            const std::shared_ptr<std::string> &value,
            const std::string &image_path = "test.png",
            const std::string &label = "String")
            : Component(id),
              m_Label(label),
              m_Value(value),
              m_ImagePath(image_path)
        {
        }

        void Render(const std::string &duplication_name)
        {
            if (m_Value)
            {
                ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(m_ImagePath);

                char buffer[256];
                std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));
                ImGui::InputTextWithImage(m_Label.c_str(), texture, buffer, sizeof(buffer));
                *m_Value = std::string(buffer);
            }
            else
            {
                ImGui::Text("INVALID VALUE");
            }
        }

        void SetImagePath(const std::string& path)
        {
            m_ImagePath = path;
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
        std::string m_ImagePath;
    };

}

#endif // UIKIT_V1_IMAGE_INPUTS
