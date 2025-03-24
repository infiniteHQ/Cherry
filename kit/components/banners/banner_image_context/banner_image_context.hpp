#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BannerImageContext
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BANNER_IMAGE_CONTEXT
#define CHERRY_KIT_BANNER_IMAGE_CONTEXT

namespace Cherry
{
    namespace Components
    {
        class BannerImageContext : public Component
        {
        public:
            BannerImageContext(const Cherry::Identifier &id, const std::vector<std::function<void()>> &renderCallbacks, const float &width = 260.0f, const float &height = 110.0f, const std::function<void()> &onClickedCallback = []() {}) : Component(id), m_RenderCallbacks(renderCallbacks), m_OnClickedCallback(onClickedCallback)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("block_color", "#252525");
                SetProperty("block_color_hovered", "#454545");
                SetProperty("block_color_pressed", "#555555");
                SetProperty("block_border_color", "#353535");
                SetProperty("block_border_color_hovered", "#353535");
                SetProperty("block_border_color_pressed", "#555555");
                SetProperty("block_border_radius", std::to_string(0.0f));
                SetProperty("block_border_size", std::to_string(1.0f));
                SetProperty("size_x", std::to_string(width));
                SetProperty("size_y", std::to_string(height));

                // Data & User-level informations
                SetData("isClicked", "false");
                SetData("isPressed", "false");
                SetData("isHovered", "false");
                SetData("isActivated", "false");
                SetData("lastClicked", "never");
                SetData("lastPressed", "never");
                SetData("lastHovered", "never");
                SetData("lastActivated", "never");
            }

            void Render() override
            {
                ImVec2 squareSize(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_x")));

                const char *originalText = "Title";
                const std::string envproject = "Title";
                const std::string desc = "Description";
                char truncatedText[32];

                if (strlen(originalText) > 24)
                {
                    strncpy(truncatedText, originalText, 8);
                    strcpy(truncatedText + 8, "...");
                }
                else
                {
                    strcpy(truncatedText, originalText);
                }

                ImVec2 textSize = CherryGUI::CalcTextSize(truncatedText);
                ImVec2 totalSize(squareSize.x, squareSize.y + textSize.y + 5);

                ImVec2 cursorPos = CherryGUI::GetCursorScreenPos();

                if (CherryGUI::InvisibleButton(originalText, totalSize))
                {
                    /*if (link_path != "")
                    {
                        OpenURL(link_path);
                    }*/
                }

                if (CherryGUI::IsItemHovered())
                {
                    CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
                }

                ImDrawList *drawList = CherryGUI::GetWindowDrawList();

                if (!envproject.empty() && std::filesystem::exists(envproject))
                {
                    drawList->AddImage(Cherry::GetTexture(envproject), cursorPos, ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y));
                }
                else
                {
                    drawList->AddImage(Cherry::GetTexture(envproject), cursorPos, ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y));
                }

                {
                    ImVec2 textPadding(10.0f, 5.0f);
                    float blurHeight = 30.0f;

                    ImVec2 blurPos = ImVec2(cursorPos.x, cursorPos.y + squareSize.y - blurHeight);
                    ImVec2 blurSize = ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y);

                    drawList->AddRectFilled(blurPos, blurSize, IM_COL32(0, 0, 0, 128));

                    ImVec2 textPos = ImVec2(cursorPos.x + textPadding.x, cursorPos.y + squareSize.y - blurHeight + textPadding.y);

                    CherryGUI::PushFont(CherryGUI::GetIO().Fonts->Fonts[1]); // Assuming font[1] is a larger font. Adjust index as needed.

                    drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), envproject.c_str());

                    CherryGUI::PopFont();
                }

                ImVec2 textPos = ImVec2(cursorPos.x, cursorPos.y + squareSize.y + 5);
                std::string fullText = desc;
                float maxWidth = squareSize.x;

                std::string line;
                Cherry::PushFont("dv-b");
                float lineWidth = 0.0f;
                for (char ch : fullText)
                {
                    ImVec2 charSize = CherryGUI::CalcTextSize(std::string(1, ch).c_str());
                    if (lineWidth + charSize.x > maxWidth && !line.empty())
                    {
                        drawList->AddText(textPos, Cherry::HexToImU32("#999999ff"), line.c_str());
                        textPos.y += charSize.y;
                        line.clear();
                        lineWidth = 0.0f;
                    }
                    line += ch;
                    lineWidth += charSize.x;
                }

                if (!line.empty())
                {
                    drawList->AddText(textPos, Cherry::HexToImU32("#999999ff"), line.c_str());
                }
                Cherry::PopFont();

                ImU32 textColor = IM_COL32(255, 255, 255, 255);
                ImU32 highlightColor = IM_COL32(255, 255, 0, 255);
                ImU32 highlightTextColor = IM_COL32(0, 0, 0, 255);

                if (CherryGUI::IsItemHovered())
                {
                    CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
                    drawList->AddRect(cursorPos, ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y), IM_COL32(135, 135, 135, 255), 0.0f, 0, 2.0f);
                }

                /*if (!disable_stack)
                {
                    float windowVisibleX2 = CherryGUI::GetWindowPos().x + CherryGUI::GetWindowContentRegionMax().x;
                    if (cursorPos.x + totalSize.x < windowVisibleX2)
                        CherryGUI::SameLine();
                }*/
            }

        private:
            std::function<void()> m_OnClickedCallback;
            std::vector<std::function<void()>> m_RenderCallbacks;
            std::string GetCurrentTime()
            {
                std::string m_LastClickTime;
                auto now = std::chrono::system_clock::now();
                std::time_t now_c = std::chrono::system_clock::to_time_t(now);
                m_LastClickTime = std::ctime(&now_c);
                m_LastClickTime.erase(m_LastClickTime.length() - 1);
                return m_LastClickTime;
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> BannerImageContext(const std::vector<std::function<void()>> &renderCallbacks)
        {
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::BannerImageContext>(Components::BannerImageContext(anonymous_id, renderCallbacks));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> BannerImageContext(const float &width = 260.0f, const float &height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {}, const std::function<void()> &onClickedCallback = []() {})
        {
            auto anonymous_id = Application::GetAnonymousID();
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                existing->Render();
                return existing;
            }
            else
            {
                auto button = Application::CreateAnonymousComponent<Components::BannerImageContext>(Components::BannerImageContext(anonymous_id, renderCallbacks, width, height, onClickedCallback));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> BannerImageContext(const Cherry::Identifier &identifier, const std::vector<std::function<void()>> &renderCallbacks)
        {
            switch (identifier.property())
            {
            case IdentifierProperty::Inline:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BannerImageContext>(Components::BannerImageContext(identifier, renderCallbacks));
                new_title->Render();
                return new_title;
                break;
            }
            case IdentifierProperty::CreateOnly:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BannerImageContext>(Components::BannerImageContext(identifier, renderCallbacks));
                return new_title;
                break;
            }
            case IdentifierProperty::None:
            {
                break;
            }
            default:
            {
                break;
            }
            }

            // Get the object if exist
            auto existing_title = Application::GetComponent(identifier);
            if (existing_title)
            {
                existing_title->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_title = Application::CreateComponent<Components::BannerImageContext>(Components::BannerImageContext(identifier, renderCallbacks));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }

        inline std::shared_ptr<Component> BannerImageContext(const Cherry::Identifier &identifier, const float &width = 260.0f, const float &height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {}, const std::function<void()> &onClickedCallback = []() {})
        {
            switch (identifier.property())
            {
            case IdentifierProperty::Inline:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BannerImageContext>(Components::BannerImageContext(identifier, renderCallbacks, width, height, onClickedCallback));
                new_title->Render();
                return new_title;
                break;
            }
            case IdentifierProperty::CreateOnly:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BannerImageContext>(Components::BannerImageContext(identifier, renderCallbacks, width, height, onClickedCallback));
                return new_title;
                break;
            }
            case IdentifierProperty::None:
            {
                break;
            }
            default:
            {
                break;
            }
            }

            // Get the object if exist
            auto existing_title = Application::GetComponent(identifier);
            if (existing_title)
            {
                existing_title->Render();
            }
            else
            {
                // Create the object if not exist
                auto new_title = Application::CreateComponent<Components::BannerImageContext>(Components::BannerImageContext(identifier, renderCallbacks, width, height));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_BANNER_IMAGE_CONTEXT
