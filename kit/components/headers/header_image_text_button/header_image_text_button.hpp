#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// HeaderImageTextButton
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_HEADER_IMAGE_TEXT_BUTTON
#define CHERRY_KIT_HEADER_IMAGE_TEXT_BUTTON

namespace Cherry
{
    namespace Components
    {
        class HeaderImageTextButton : public Component
        {
        public:
            HeaderImageTextButton(const Cherry::Identifier &id, const std::string &label, const std::string &image_path, const std::function<void()> &render_content)
                : Component(id), m_RenderContent(render_content)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("button_color_border", "#454545B2");
                SetProperty("button_color_border_hovered", "#454545B2");
                SetProperty("button_color_border_clicked", "#454545B2");
                SetProperty("button_color_bg", "#242424FF");
                SetProperty("button_color_bg_hovered", "#343434FF");
                SetProperty("button_color_bg_clicked", "#444444FF");
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
                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }
                ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

                ImGuiStyle &style = ImGui::GetStyle();
                float texture_size = ImGui::GetFontSize();
                float padding = style.ItemInnerSpacing.x;

                ImGui::BeginGroup();

                CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("color_bg_hovered")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("color_bg_clicked")));

                std::string button_label = Label + "_button";

                std::cout << Label << std::endl;
                bool isOpened = GetData("isOpened") == "true";

                if (ImGui::ImageSizeButtonWithText(Cherry::GetTexture(GetProperty("image_path")), 700.0f, button_label.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
                {
                    SetData("isOpened", isOpened ? "false" : "true");
                }

                ImGui::PopStyleColor(4);

                ImVec2 arrow_pos = ImVec2(ImGui::GetItemRectMax().x - style.FramePadding.x - ImGui::GetFontSize(), ImGui::GetItemRectMin().y + style.FramePadding.y + 3.0f);
                ImGui::RenderArrow(ImGui::GetWindowDrawList(), arrow_pos, ImGui::GetColorU32(ImGuiCol_Text), GetData("isOpened") == "true" ? ImGuiDir_Down : ImGuiDir_Right);

                ImVec2 button_size = ImGui::GetItemRectSize();
                ImVec2 button_pos = ImGui::GetItemRectMin();
                ImVec2 content_pos;
                content_pos.x = button_pos.x + button_size.x - texture_size - 100;
                content_pos.y = button_pos.y;

                ImGui::SetCursorScreenPos(content_pos);
                ImVec2 child_size = ImVec2(100, button_size.y);

                ImGui::PushStyleColor(ImGuiCol_ChildBg, Cherry::HexToImU32("#00000000"));

                std::string child_label = "###child" + identifier;
                ImGui::BeginChild(child_label.c_str(), child_size, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

                CherryGUI::PushStyleColor(ImGuiCol_Border, HexToRGBA(GetProperty("button_color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_Button, HexToRGBA(GetProperty("button_color_border")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, HexToRGBA(GetProperty("button_color_bg_hovered")));
                CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, HexToRGBA(GetProperty("button_color_bg_clicked")));

                ImGui::Button("Testqsd");

                ImGui::PopStyleColor(4);

                ImGui::EndChild();
                ImGui::PopStyleColor();

                ImGui::EndGroup();

                ImGui::PopStyleVar();

                if (GetData("isOpened") == "true")
                {
                    CherryID::UpgradeIncrementorLevel();
                    if (m_RenderContent)
                    {
                        m_RenderContent();
                    }
                    CherryID::DowngradeIncrementorLevel();
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
        bool HeaderImageTextButton(const std::string &label, const std::string &image_path, const std::function<void()> &render_content = []() {})
        {
            // Inline component
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing->GetData("isClicked") == "true" ? true : false;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::HeaderImageTextButton>(Components::HeaderImageTextButton(anonymous_id, label, image_path, render_content));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        bool HeaderImageTextButton(const Cherry::Identifier &identifier, const std::string &label, const std::string &image_path, const std::function<void()> &render_content = []() {})
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
                auto new_button = Application::CreateComponent<Components::HeaderImageTextButton>(Components::HeaderImageTextButton(identifier, label, image_path, render_content));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_HEADER_IMAGE_TEXT_BUTTON
