#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_STRING
#define CHERRY_KIT_KEYVAL_STRING

namespace Cherry {
  namespace Components {
    class KeyValString : public Component {
     public:
      KeyValString(const Cherry::Identifier &id, const std::string &label, std::string *value)
          : Component(id),
            m_Value(value) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_text", "#454545B2");

        // Informations
        SetProperty("label", label);
      }

      void Render() override {
        auto parent = Cherry::GetParent();
        if (parent) {
          if (std::stoi(parent->GetData("renderedColumn")) == 0) {
            CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
            CherryGUI::AlignTextToFramePadding();
            CherryGUI::Indent(10.0f);
            CherryGUI::TextWrapped(GetProperty("label").c_str());
            CherryGUI::Unindent(10.0f);
          } else if (std::stoi(parent->GetData("renderedColumn")) == 1) {
            std::string identifier = GetIdentifier().string();
            std::string Label = "####" + GetProperty("label");

            if (!identifier.empty()) {
              Label += Label + identifier;
            }

            CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));

            if (m_Value) {
              CherryGUI::SetNextItemWidth(-FLT_MIN);

              char buffer[256];
              std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));

              CherryGUI::InputText(Label.c_str(), buffer, sizeof(buffer));
              *m_Value = std::string(buffer);
            } else {
              CherryGUI::Text("INVALID VALUE");
            }
          }
        }
      }

     private:
      std::string *m_Value;
    };
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline std::shared_ptr<Component> KeyValString(const std::string &label, std::string *value) {
      // Inline component
      auto anonymous_id = Application::GenerateUniqueID(label, value);
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing;
      } else {
        auto button = Application::CreateAnonymousComponent<Components::KeyValString>(
            Components::KeyValString(anonymous_id, label, value));
        button->Render();
        return button;
      }
    }

    inline std::shared_ptr<Component>
    KeyValString(const Cherry::Identifier &identifier, const std::string &label, std::string *value) {
      // Get the object if exist
      auto existing_keyval = Application::GetComponent(identifier);
      if (existing_keyval) {
        existing_keyval->RefreshContextProperties();
        existing_keyval->Render();
      } else {
        // Create the object if not exist
        auto new_keyval =
            Application::CreateComponent<Components::KeyValString>(Components::KeyValString(identifier, label, value));
        new_keyval->RefreshContextProperties();
        new_keyval->Render();
        return new_keyval;
      }
      return existing_keyval;
    }
  }  // namespace Kit

}  // namespace Cherry

#endif  // CHERRY_KIT_KEYVAL_STRING
