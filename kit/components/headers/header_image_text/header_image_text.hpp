#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// HeaderImageText
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_HEADER_IMAGE_TEXT
#define CHERRY_KIT_HEADER_IMAGE_TEXT

namespace Cherry
{
    namespace Components
    {
        class HeaderImageText : public Component
        {
        public:
            HeaderImageText(const Cherry::Identifier &id, const std::string &label, const std::string &image_path, const std::function<void()> &render_content)
                : Component(id), m_RenderContent(render_content)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_border", "#454545B2");
                SetProperty("color_border_hovered", "#454545B2");
                SetProperty("color_border_clicked", "#454545B2");
                SetProperty("color_bg", "#242424FF");
                SetProperty("color_bg_hovered", "#343434FF");
                SetProperty("color_bg_clicked", "#444444FF");

                // Sizes
                SetProperty("size_x", "6");
                SetProperty("size_y", "6");

                // Images
                SetProperty("image_path", image_path);

                // Informations
                SetProperty("label", label);

                // Data & User-level informations
                SetData("lastClicked", "never");
                SetData("isClicked", "false");
            }

            void Render() override
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

                CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("color_bg_hovered")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("color_bg_clicked")));

                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                bool *p_open = ImGui::GetStateStorage()->GetBoolRef(ImGui::GetID(Label.c_str()), false);

                ImGuiStyle &style = ImGui::GetStyle();

                float texture_size = ImGui::GetFontSize();
                float padding = style.ItemInnerSpacing.x;

                ImGui::BeginGroup();
                if (ImGui::ImageSizeButtonWithText(Cherry::GetTexture(GetProperty("image_path")), 700.0f, Label.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
                {
                    *p_open ^= 1;
                }

                ImVec2 arrow_pos = ImVec2(ImGui::GetItemRectMax().x - style.FramePadding.x - ImGui::GetFontSize(), ImGui::GetItemRectMin().y + style.FramePadding.y + 3.0f);
                ImGui::RenderArrow(ImGui::GetWindowDrawList(), arrow_pos, ImGui::GetColorU32(ImGuiCol_Text), *p_open ? ImGuiDir_Down : ImGuiDir_Right);
                ImGui::EndGroup();

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();

                if (*p_open)
                {
                    if (m_RenderContent)
                    {
                        m_RenderContent();
                    }
                }
            }

        private:
            void UpdateLastClickTime()
            {
                std::string m_LastClickTime;
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                m_LastClickTime = std::ctime(&now_c);
                m_LastClickTime.erase(m_LastClickTime.length() - 1);
                SetData("lastClicked", m_LastClickTime);
            }

            std::function<void()> m_RenderContent;
        };
    }

    // End-User API
    namespace Kit
    {
        bool HeaderImageText(const std::string &label, const std::string &image_path, const std::function<void()> &render_content = []() {})
        {
            // Inline component
            auto button = Application::CreateAnonymousComponent<Components::HeaderImageText>(Components::HeaderImageText(Cherry::Identifier(""), label, image_path, render_content));
            button->Render();
            return button->GetData("isClicked") == "true" ? true : false;
        }

        bool HeaderImageText(const Cherry::Identifier &identifier, const std::string &label, const std::string &image_path, const std::function<void()> &render_content = []() {})
        {
            // Get the object if exist
            auto existing_button = Application::GetComponent(identifier);
            if (existing_button)
            {
                existing_button->Render();
                return existing_button->GetData("isClicked") == "true" ? true : false;
            }
            else
            {
                // Create the object if not exist
                auto new_button = Application::CreateComponent<Components::HeaderImageText>(Components::HeaderImageText(identifier, label, image_path, render_content));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_HEADER_IMAGE_TEXT
