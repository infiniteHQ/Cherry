#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BlockVerticalCustom
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BLOCK_VERTICAL_CUSTOM
#define CHERRY_KIT_BLOCK_VERTICAL_CUSTOM

namespace Cherry
{
    namespace Components
    {
        class BlockVerticalCustom : public Component
        {
        public:
            BlockVerticalCustom(const Cherry::Identifier &id, const std::vector<std::function<void()>> &renderCallbacks, const float &width = 260.0f, const float &height = 110.0f, const std::function<void()> &onClickedCallback = []() {}) : Component(id), m_RenderCallbacks(renderCallbacks), m_OnClickedCallback(onClickedCallback)
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
                static ImVec2 cachedSize;
                static ImU32 background_color;
                static ImU32 background_color_hovered;
                static ImU32 background_color_pressed;
                static ImU32 border_color;
                static ImU32 border_color_hovered;
                static ImU32 border_color_pressed;
                static float border_size;
                static float cached_radius;
                static bool disabletime;

                if (NeedRefreshing())
                {
                    cachedSize = ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y")));

                    background_color = HexToImU32(GetProperty("block_color"));
                    background_color_hovered = HexToImU32(GetProperty("block_color_hovered"));
                    background_color_pressed = HexToImU32(GetProperty("block_color_pressed"));

                    border_color = HexToImU32(GetProperty("block_border_color"));
                    border_color_hovered = HexToImU32(GetProperty("block_border_color_hovered"));
                    border_color_pressed = HexToImU32(GetProperty("block_border_color_pressed"));

                    border_size = std::stof(GetProperty("block_border_size"));
                    cached_radius = std::stof(GetProperty("block_border_radius"));
                    disabletime = GetProperty("disable_time") == "false";

                    Refreshed();
                }

                ImVec2 pos = CherryGUI::GetCursorScreenPos();

                CherryGUI::PushStyleVar(ImGuiStyleVar_ChildRounding, cached_radius);
                CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color);

                int style_props_opt = 0;

                bool isHovered = GetData("isHovered") == "true";
                bool isClicked = GetData("isClicked") == "true";
                bool isActivated = GetData("isActivated") == "true";
                bool isPressed = GetData("isPressed") == "true";

                if (isHovered)
                {
                    if (disabletime)
                        SetData("lastHovered", GetCurrentTime());
                    CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_hovered);
                    border_color = border_color_hovered;
                    style_props_opt++;
                }

                if (isClicked)
                {
                    if (disabletime)
                        SetData("lastClicked", GetCurrentTime());
                    CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_pressed);
                    border_color = border_color_pressed;
                    style_props_opt++;
                }

                if (isActivated)
                {
                    if (disabletime)
                        SetData("lastActivated", GetCurrentTime());
                    CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_pressed);
                    border_color = border_color_pressed;
                }

                if (isPressed)
                {
                    if (disabletime)
                        SetData("lastPressed", GetCurrentTime());
                    CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_pressed);
                    border_color = border_color_pressed;
                    style_props_opt++;
                }

                SetData("isHovered", "false");
                SetData("isClicked", "false");
                SetData("isPressed", "false");
                SetData("isActivated", "false");

                if (CherryGUI::BeginChild("RenderCallbacks", cachedSize, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
                {
                    CherryGUI::InvisibleButton(GetIdentifier().string().c_str(), cachedSize);

                    if (CherryGUI::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && CherryGUI::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        SetData("isClicked", "true");
                        if (m_OnClickedCallback)
                        {
                            m_OnClickedCallback();
                        }
                    }

                    if (CherryGUI::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
                    {
                        CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
                        SetData("isHovered", "true");
                    }

                    if (CherryGUI::IsItemActivated())
                    {
                        SetData("isActivated", "true");
                    }

                    if (CherryGUI::IsItemActive())
                    {
                        SetData("isPressed", "true");
                    }

                    CherryGUI::SetCursorScreenPos(pos);
                    CherryGUI::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
                    CherryGUI::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

                    for (auto &callback : m_RenderCallbacks)
                    {
                        callback();
                    }

                    CherryGUI::PopStyleColor();
                    CherryGUI::PopStyleVar();
                }
                CherryGUI::EndChild();

                CherryGUI::PopStyleVar();
                CherryGUI::PopStyleColor();
                CherryGUI::PopStyleColor(style_props_opt);

                ImDrawList *drawList = CherryGUI::GetWindowDrawList();
                drawList->AddRect(pos, ImVec2(pos.x + cachedSize.x, pos.y + cachedSize.y), border_color, cached_radius, 0, border_size);
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
        inline std::shared_ptr<Component> BlockVerticalCustom(const std::vector<std::function<void()>> &renderCallbacks)
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
                auto button = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(anonymous_id, renderCallbacks));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> BlockVerticalCustom(const float &width = 260.0f, const float &height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {}, const std::function<void()> &onClickedCallback = []() {})
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
                auto button = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(anonymous_id, renderCallbacks, width, height, onClickedCallback));
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> BlockVerticalCustom(const Cherry::Identifier &identifier, const std::vector<std::function<void()>> &renderCallbacks)
        {
            switch (identifier.property())
            {
            case IdentifierProperty::Inline:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks));
                new_title->Render();
                return new_title;
                break;
            }
            case IdentifierProperty::CreateOnly:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks));
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
                auto new_title = Application::CreateComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }

        inline std::shared_ptr<Component> BlockVerticalCustom(const Cherry::Identifier &identifier, const float &width = 260.0f, const float &height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {}, const std::function<void()> &onClickedCallback = []() {})
        {
            switch (identifier.property())
            {
            case IdentifierProperty::Inline:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(Identifier(Identifier::GetUniqueIndex()), renderCallbacks, width, height, onClickedCallback));
                new_title->Render();
                return new_title;
                break;
            }
            case IdentifierProperty::CreateOnly:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(Identifier(Identifier::GetUniqueIndex()), renderCallbacks, width, height, onClickedCallback));
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
                auto new_title = Application::CreateComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks, width, height));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_BLOCK_VERTICAL_CUSTOM
