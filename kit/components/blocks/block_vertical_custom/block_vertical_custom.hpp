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
            BlockVerticalCustom(const Cherry::Identifier &id, const std::vector<std::function<void()>> &renderCallbacks, const float& width = 260.0f, const float& height = 110.0f)
                : Component(id), m_RenderCallbacks(renderCallbacks)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("color_block", "#353535");
                SetProperty("size_x", std::to_string(width));
                SetProperty("size_y", std::to_string(height));
            }

            void Render() override
            {
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImVec2 size = ImVec2(std::stof(GetProperty("size_x")), std::stof(GetProperty("size_y")));
                ImU32 color = HexToImU32(GetProperty("color_block"));

                ImVec4 colVec = ImGui::ColorConvertU32ToFloat4(color);
                ImU32 flapColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colVec.x * 0.8f, colVec.y * 0.8f, colVec.z * 0.8f, colVec.w));
                ImU32 shadowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(colVec.x * 0.7f, colVec.y * 0.7f, colVec.z * 0.7f, colVec.w * 0.2f));

                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

                if (ImGui::BeginChild("RenderCallbacks", size, false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
                {
                    ImVec2 pos = ImGui::GetItemRectMin();

                    ImGui::InvisibleButton(GetIdentifier().string().c_str(), size);
                    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                    {
                        // ChangeDirectory(path);
                    }

                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
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
            }

        private:
            std::vector<std::function<void()>> m_RenderCallbacks;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> BlockVerticalCustom(const std::vector<std::function<void()>> &renderCallbacks)
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(Cherry::Identifier(""), renderCallbacks));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> BlockVerticalCustom(const float& width = 260.0f, const float& height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {})
        {
            // Inline component
            auto title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(Cherry::Identifier(""), renderCallbacks, width, height));
            title->Render();
            return title;
        }

        inline std::shared_ptr<Component> BlockVerticalCustom(const Cherry::Identifier &identifier, const std::vector<std::function<void()>> &renderCallbacks)
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks));
                new_title->Render();
                return new_title;
            }            
            
            if (identifier.string() == "__create")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks));
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
                auto new_title = Application::CreateComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }


        inline std::shared_ptr<Component> BlockVerticalCustom(const Cherry::Identifier &identifier, const float& width = 260.0f, const float& height = 110.0f, const std::vector<std::function<void()>> &renderCallbacks = {})
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks, width, height));
                new_title->Render();
                return new_title;
            }
            
            if (identifier.string() == "__create")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks, width, height));
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
                auto new_title = Application::CreateComponent<Components::BlockVerticalCustom>(Components::BlockVerticalCustom(identifier, renderCallbacks, width, height));
                new_title->Render();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_BLOCK_VERTICAL_CUSTOM
