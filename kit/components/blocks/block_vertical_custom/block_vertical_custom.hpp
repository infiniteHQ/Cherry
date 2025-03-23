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
            BlockVerticalCustom(const Cherry::Identifier &id, const std::vector<std::function<void()>> &renderCallbacks, const float &width = 260.0f, const float &height = 110.0f)
                : Component(id), m_RenderCallbacks(renderCallbacks)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("block_color", "#88FF88");
                SetProperty("block_color_hovered", "#FF8888");
                SetProperty("block_color_pressed", "#8888FF");
                SetProperty("block_border_color", "#22FF22");
                SetProperty("block_border_color_hovered", "#FF2222");
                SetProperty("block_border_color_pressed", "#2222FF");
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
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 size = ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y")));

                ImU32 backgroundColor = HexToImU32(GetProperty("block_color"));
                ImU32 backgroundColorHovered = HexToImU32(GetProperty("block_color_hovered"));
                ImU32 backgroundColorPressed = HexToImU32(GetProperty("block_color_pressed"));

                ImU32 borderColor = HexToImU32(GetProperty("block_border_color"));
                ImU32 borderColorHovered = HexToImU32(GetProperty("block_border_color_hovered"));
                ImU32 borderColorPressed = HexToImU32(GetProperty("block_border_color_pressed"));

                float borderSize = std::stof(GetProperty("block_border_size"));
                float radius = std::stof(GetProperty("block_border_radius"));

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, radius);
                ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColor);

                int style_props_opt = 0;

                if (GetData("isHovered") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastHovered", GetCurrentTime());
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColorHovered);
                    borderColor = borderColorHovered;
                    style_props_opt++;
                }

                if (GetData("isClicked") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastClicked", GetCurrentTime());
                    std::cout << GetData("lastClicked") << std::endl;

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColorPressed);
                    borderColor = borderColorPressed;
                    style_props_opt++;
                }

                if (GetData("isActivated") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastActivated", GetCurrentTime());
                        
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColorPressed);
                    borderColor = borderColorPressed;
                }

                if (GetProperty("isPressed") == "true")
                {
                    if (GetProperty("disable_time") == "false")
                        SetData("lastPressed", GetCurrentTime());

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, backgroundColorPressed);
                    borderColor = borderColorPressed;
                    style_props_opt++;
                }

                SetData("isHovered", "false");
                SetData("isClicked", "false");
                SetData("isPressed", "false");
                SetData("isActivated", "false");

                if (ImGui::BeginChild("RenderCallbacks", size, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
                {
                    ImGui::InvisibleButton(GetIdentifier().string().c_str(), size);

                    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                    {
                        SetData("isClicked", "true");
                    }

                    if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
                    {
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                        SetData("isHovered", "true");
                    }

                    if (ImGui::IsItemActivated())
                    {
                        SetData("isActivated", "true");
                    }

                    if (ImGui::IsItemActive())
                    {
                        SetData("isPressed", "true");
                    }

                    ImGui::SetCursorScreenPos(pos);
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));

                    for (auto &callback : m_RenderCallbacks)
                    {
                        callback();
                    }

                    ImGui::PopStyleColor();
                    ImGui::PopStyleVar();
                }
                ImGui::EndChild();

                ImGui::PopStyleVar();
                ImGui::PopStyleColor();
                ImGui::PopStyleColor(style_props_opt);

                ImDrawList *drawList = ImGui::GetWindowDrawList();
                drawList->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), borderColor, radius, 0, borderSize);
            }

        private:
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

        inline std::shared_ptr<Component> BlockVerticalCustom(const float &width = 260.0f, const float &height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {})
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
                auto button = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(anonymous_id, renderCallbacks, width, height));
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

        inline std::shared_ptr<Component> BlockVerticalCustom(const Cherry::Identifier &identifier, const float &width = 260.0f, const float &height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {})
        {
            switch (identifier.property())
            {
            case IdentifierProperty::Inline:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks, width, height));
                new_title->Render();
                return new_title;
                break;
            }
            case IdentifierProperty::CreateOnly:
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks, width, height));
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
