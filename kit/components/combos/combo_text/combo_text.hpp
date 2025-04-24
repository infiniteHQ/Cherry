#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ComboText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_COMBO_TEXT
#define CHERRY_KIT_COMBO_TEXT

namespace Cherry {
  namespace Components {
    class ComboText : public Component {
     public:
      ComboText(const Cherry::Identifier &id, const std::string &label, std::vector<std::string> *list, int default_index)
          : Component(id),
            m_List(list) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_border", "#454545B2");
        SetProperty("color_border_hovered", "#454545B2");
        SetProperty("color_border_clicked", "#454545B2");
        SetProperty("color_bg", "#242424FF");
        SetProperty("color_bg_hovered", "#343434FF");
        SetProperty("color_bg_clicked", "#444444FF");

        // Images
        SetProperty("default_index", std::to_string(default_index));
        SetProperty("selected", std::to_string(default_index));

        // Informations
        SetProperty("label", label);

        // Size
        SetProperty("x_padding", "7");
        SetProperty("y_padding", "7");

        // Data & User-level informations
        SetData("lastClicked", "never");
        SetData("isClicked", "false");
      }

      void Render() override {
        static ImGuiComboFlags flags = 0;

        int selected = std::stoi(GetProperty("selected"));
        int default_index = std::stoi(GetProperty("default_index"));
        ImGui::PushStyleVar(
            ImGuiStyleVar_FramePadding, ImVec2(std::stoi(GetProperty("x_padding")), std::stoi(GetProperty("y_padding"))));

        if (default_index < 0 || default_index >= m_List->size())
          default_index = 0;

        const char *combo_preview_value = (*m_List)[selected].c_str();

        std::string identifier = GetIdentifier().string();
        std::string Label = GetProperty("label");

        if (!identifier.empty()) {
          Label += "####" + identifier;
        }

        if (CherryGUI::BeginCombo(Label.c_str(), combo_preview_value, flags)) {
          for (int n = 0; n < (*m_List).size(); n++) {
            const bool is_selected = (selected == n);
            if (CherryGUI::Selectable((*m_List)[n].c_str(), is_selected)) {
              if (selected != n) {
                SetProperty("selectedString", std::to_string(n));
                // UpdateLastChangedTime();
              }
            }

            if (is_selected)
              CherryGUI::SetItemDefaultFocus();
          }
          CherryGUI::EndCombo();
        }
        ImGui::PopStyleVar();
      }

     private:
      std::vector<std::string> *m_List;
    };
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline std::shared_ptr<Component>
    ComboText(const std::string &label, std::vector<std::string> *list, int default_index = 0) {
      auto anonymous_id = Application::GetAnonymousID(label);
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing;
      } else {
        auto title = Application::CreateAnonymousComponent<Components::ComboText>(
            Components::ComboText(anonymous_id, label, list, default_index));
        title->Render();
        return title;
      }
    }

    inline std::shared_ptr<Component> ComboText(
        const Cherry::Identifier &identifier,
        const std::string &label,
        std::vector<std::string> *list,
        int default_index = 0) {
      if (identifier.string() == "__inline") {
        auto new_title =
            std::make_shared<Components::ComboText>(Components::ComboText(identifier, label, list, default_index));
        new_title->Render();
        return new_title;
      }

      // Get the object if exist
      auto existing_title = Application::GetComponent(identifier);
      if (existing_title) {
        existing_title->Render();
      } else {
        // Create the object if not exist
        auto new_title = Application::CreateComponent<Components::ComboText>(
            Components::ComboText(identifier, label, list, default_index));
        new_title->Render();
        return new_title;
      }
      return existing_title;
    }
  }  // namespace Kit
}  // namespace Cherry

#endif  // CHERRY_KIT_COMBO_TEXT
