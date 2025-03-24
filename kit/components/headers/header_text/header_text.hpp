#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// HeaderText
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_HEADER_TEXT
#define CHERRY_KIT_HEADER_TEXT

namespace Cherry
{
    namespace Components
    {
        class HeaderText : public Component
        {
        public:
            HeaderText(const Cherry::Identifier &id, const std::string &label, const std::function<void()> &render_content)
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

                // Informations
                SetProperty("label", label);

                // Data & User-level informations
                SetData("isOpened", "false");
            }

            void Render() override
            {
                CherryGUI::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

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

                ImGuiStyle &style = CherryGUI::GetStyle();

                float texture_size = CherryGUI::GetFontSize();
                float padding = style.ItemInnerSpacing.x;

                CherryGUI::BeginGroup();

                bool isOpened = GetData("isOpened") == "true";

                if (CherryGUI::Button(Label.c_str(), ImVec2(700.0f, 0.0f)))
                {
                    SetData("isOpened", isOpened ? "false" : "true");
                    SetData("isUpdated", "true");
                }

                ImVec2 arrow_pos = ImVec2(CherryGUI::GetItemRectMax().x - style.FramePadding.x - CherryGUI::GetFontSize(), CherryGUI::GetItemRectMin().y + style.FramePadding.y);
                CherryGUI::RenderArrow(CherryGUI::GetWindowDrawList(), arrow_pos, CherryGUI::GetColorU32(ImGuiCol_Text), GetData("isOpened") == "true" ? ImGuiDir_Down : ImGuiDir_Right);
                CherryGUI::EndGroup();

                CherryGUI::PopStyleColor(4);
                CherryGUI::PopStyleVar();

                if (GetData("isOpened") == "true")
                {
                    CherryID::UpgradeIncrementorLevel();
                    if (m_RenderContent)
                    {
                        m_RenderContent();
                    }
                    CherryID::DowngradeIncrementorLevel();
                }

                SetData("isUpdated", "false");
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
        inline bool HeaderText(const std::string &label, const std::function<void()> &render_content = []() {})
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
                auto button = Application::CreateAnonymousComponent<Components::HeaderText>(Components::HeaderText(anonymous_id, label, render_content));
                button->Render();
                return button->GetData("isClicked") == "true" ? true : false;
            }
            return false;
        }

        inline bool HeaderText(const Cherry::Identifier &identifier, const std::string &label, const std::function<void()> &render_content = []() {})
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
                auto new_button = Application::CreateComponent<Components::HeaderText>(Components::HeaderText(identifier, label, render_content));
                new_button->Render();
                return new_button->GetData("isClicked") == "true" ? true : false;
            }
        }
    }
}

#endif // CHERRY_KIT_HEADER_TEXT
