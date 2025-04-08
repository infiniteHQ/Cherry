#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ModalSimple
//

#ifndef CHERRY_KIT_MODAL_TITLE
#define CHERRY_KIT_MODAL_TITLE

namespace Cherry
{
    namespace Components
    {
        class ModalSimple : public Component
        {
        public:
            ModalSimple(const Cherry::Identifier &id, const std::string &title, bool *open, const std::function<void()> &callback)
                : Component(id), activated(open), render_callback(callback)
            {
                // Identifier
                SetIdentifier(id);

                // Colors
                SetProperty("padding_x", "6.0"); // TODO get the default theme
                SetProperty("padding_y", "6.0"); // TODO get the default theme

                // Informations
                SetProperty("title", title);

                if (activated)
                {
                    SetProperty("activated", std::to_string(*activated));
                }
            }

            void Render() override
            {
                if (NeedRefreshing())
                {
                    cached_title = GetProperty("title");
                    padding_y = std::stof(GetProperty("padding_y"));
                    padding_x = std::stof(GetProperty("padding_x"));

                    /// Refreshed();
                }

                if (!activated)
                    return;

                if (*activated)
                {
                    if (GetProperty("activated") == "false")
                    {
                        ImGui::CloseCurrentPopup();
                        *activated = false;
                        SetProperty("activated", std::to_string(*activated));
                    }

                    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding_x, padding_y));
                    ImGui::OpenPopup(cached_title.c_str());

                    ImVec2 main_window_size = ImGui::GetWindowSize();
                    ImVec2 window_pos = ImGui::GetWindowPos();

                    ImGui::SetNextWindowPos(ImVec2(window_pos.x + (main_window_size.x * 0.5f) - 300, window_pos.y + 150));

                    ImGui::SetNextWindowSize(ImVec2(600, 0), ImGuiCond_Always);
                    // ImGuiWindowFlags_NoMove
                    if (ImGui::BeginPopupModal(cached_title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar))
                    {
                        if (render_callback)
                        {
                            render_callback();
                        }
                        ImGui::EndPopup();
                    }

                    ImGui::PopStyleVar();
                }
            }

        private:
            std::function<void()> render_callback;
            ImVec4 cached_text_color;
            std::string cached_title;
            float font_scale;
            float padding_x;
            float padding_y;
            bool *activated;
        };
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> ModalSimple(const std::string &title, bool *activated, const std::function<void()> &render_callback = []() {})
        {
            auto anonymous_id = Application::GenerateUniqueID(title, activated);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                CherryApp.PushCurrentComponent(existing);
                existing->RenderWrapper();
                CherryApp.PopCurrentComponent();
                return existing;
            }

            std::cout << "CRE" << std::endl;
            auto button = Application::CreateAnonymousComponent<Components::ModalSimple>(Components::ModalSimple(anonymous_id, title, activated, render_callback));
            CherryApp.PushCurrentComponent(button);
            button->RenderWrapper();
            CherryApp.PopCurrentComponent();

            return button;
        }

        inline std::shared_ptr<Component> ModalSimple(const Cherry::Identifier &identifier, const std::string &title, bool *activated, const std::function<void()> &render_callback = []() {})
        {
            if (identifier.string() == "__inline")
            {
                auto new_title = Application::CreateAnonymousComponent<Components::ModalSimple>(Components::ModalSimple(identifier, title, activated, render_callback));
                CherryApp.PushCurrentComponent(new_title);
                new_title->RenderWrapper();
                CherryApp.PopCurrentComponent();
                return new_title;
            }

            // Get the object if exist
            auto existing_title = Application::GetComponent(identifier);
            if (existing_title)
            {
                CherryApp.PushCurrentComponent(existing_title);
                existing_title->RenderWrapper();
                CherryApp.PopCurrentComponent();
            }
            else
            {
                // Create the object if not exist
                auto new_title = Application::CreateComponent<Components::ModalSimple>(Components::ModalSimple(identifier, title, activated, render_callback));
                CherryApp.PushCurrentComponent(new_title);
                new_title->RenderWrapper();
                CherryApp.PopCurrentComponent();
                return new_title;
            }
            return existing_title;
        }
    }

}

#endif // CHERRY_KIT_MODAL_TITLE
