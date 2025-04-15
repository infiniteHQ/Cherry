#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"
#include "../../../../lib/imgui/misc/text_editors/text_editor.hpp"

//
// TextArea
//

#ifndef CHERRY_KIT_TEXT_AREA
#define CHERRY_KIT_TEXT_AREA

namespace Cherry
{
    namespace Components
    {
        class TextArea : public Component
        {
        public:
            TextArea(const Cherry::Identifier &id, float* width, float* height, const std::string& buffer)
                : Component(id), m_Width(width), m_Height(height), m_TextEditor(buffer)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_text", "#FFFFFFFF"); 
            }

            void Render() override
            {
                if(!m_Width || !m_Height)
                {
                    return;
                }

                m_TextEditor.render();

                ImVec2 area_size(*m_Width, *m_Height);


            }

            private: 
            float* m_Width;
            float* m_Height;
            TextEditor m_TextEditor;   
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> TextArea(float* width, float* height, const std::string& buffer)
        {
            // Inline component
            auto anonymous_id = Application::GenerateUniqueID(width, height, buffer);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {

                auto button = Application::CreateAnonymousComponent<Components::TextArea>(Components::TextArea(anonymous_id, width, height, buffer));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> TextArea(const Cherry::Identifier &identifier, float* width, float* height, const std::string& buffer)
        {
            // Get the object if exist
            auto existing_text = Application::GetComponent(identifier);
            if (existing_text)
            {
                existing_text->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_text = Application::CreateComponent<Components::TextArea>(Components::TextArea(identifier, width, height, buffer));
                new_text->Render();
                return new_text;
            }
            return existing_text;
        }
    }

}

#endif // CHERRY_KIT_TEXT_AREA
