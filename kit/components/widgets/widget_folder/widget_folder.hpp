#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// WidgetFolder
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_WIDGET_FOLDER
#define CHERRY_KIT_WIDGET_FOLDER

namespace Cherry
{
    namespace Components
    {
        class WidgetFolder : public Component
        {
        public:
            WidgetFolder(const Cherry::Identifier &id, const std::string &color = "#CC9900", const float &size_x = 150.0f, const float &size_y = 150.0f)
                : Component(id)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_folder", color);
                SetProperty("size_x", std::to_string(size_x));
                SetProperty("size_y", std::to_string(size_y));
            }

            void Render() override
            {
                ImVec2 pos = ImGui::GetCursorScreenPos();

                ImVec2 size = ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y")));
                ImU32 color = HexToImU32(GetProperty("color_folder"));

                ImVec4 colVec = ImGui::ColorConvertU32ToFloat4(color);
                ImU32 flapColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colVec.x * 0.8f, colVec.y * 0.8f, colVec.z * 0.8f, colVec.w));
                ImU32 shadowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colVec.x * 0.7f, colVec.y * 0.7f, colVec.z * 0.7f, colVec.w));

                ImGui::InvisibleButton(GetIdentifier().string().c_str(), size);
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                {
                    // ChangeDirectory(path);
                }

                ImDrawList *drawList = ImGui::GetWindowDrawList();

                float flapHeight = size.y * 0.25f;
                float flapWidth = size.x * 0.4f;
                float borderRadius = size.y * 0.05f;

                ImVec2 flapTopLeft = ImVec2(pos.x + size.x * 0.001f, pos.y);
                ImVec2 flapBottomRight = ImVec2(flapTopLeft.x + flapWidth, pos.y + flapHeight);

                ImVec2 flapBarTopLeft = ImVec2(pos.x, pos.y + flapHeight - size.y * 0.14f);
                ImVec2 flapBarBottomRight = ImVec2(pos.x + size.x, pos.y + flapHeight + size.y * 0.07f);

                ImVec2 bodyTopLeft = ImVec2(pos.x, pos.y + flapHeight);
                ImVec2 bodyBottomRight = ImVec2(pos.x + size.x, pos.y + size.y);

                drawList->AddRectFilled(flapTopLeft, flapBottomRight, flapColor, borderRadius, ImDrawFlags_RoundCornersTopLeft);
                drawList->AddRectFilled(flapBarTopLeft, flapBarBottomRight, flapColor);

                drawList->AddRectFilled(bodyTopLeft, bodyBottomRight, color, borderRadius);

                float lineOffset = size.y * 0.05f;
                for (int i = 1; i <= 3; i++)
                {
                    float yPos = bodyBottomRight.y - (lineOffset * i);
                    drawList->AddLine(ImVec2(bodyTopLeft.x + size.x * 0.1f, yPos),
                                      ImVec2(bodyBottomRight.x - size.x * 0.1f, yPos),
                                      shadowColor, 2.0f);
                }

                if (ImGui::IsItemHovered())
                {
                    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
                }
            }
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> WidgetFolder(const std::string &color = "#CC9900", const float &size_x = 150.0f, const float &size_y = 150.0f)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::WidgetFolder>(Components::WidgetFolder(Cherry::Identifier(""), color, size_x, size_y));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> WidgetFolder(const Cherry::Identifier &identifier, const std::string &color = "#CC9900", const float &size_x = 150.0f, const float &size_y = 150.0f)
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::WidgetFolder>(Components::WidgetFolder(identifier, color, size_x, size_y));
                new_title->Render();
                return new_title;
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
                auto new_title = Application::CreateComponent<Components::WidgetFolder>(Components::WidgetFolder(identifier, color, size_x, size_y));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_WIDGET_FOLDER
