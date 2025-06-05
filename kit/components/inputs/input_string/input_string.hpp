#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_STRING
#define CHERRY_KIT_INPUT_STRING

namespace Cherry {
  namespace Components {
    class InputString : public Component {
     public:
      InputString(const Cherry::Identifier &id, const std::string &label, std::string *value)
          : Component(id),
            m_Value(value) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_text", "#454545B2");
        SetProperty("padding_x", "7");
        SetProperty("padding_y", "7");

        // Informations
        SetProperty("label", label);
      }

      void Render() override {
        std::string identifier = GetIdentifier().string();
        std::string Label = GetProperty("label");

        if (!identifier.empty()) {
          Label += "####" + identifier;
        }

        if (m_Value) {
          char buffer[256];
          std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));

          CherryGUI::PushStyleVar(
              ImGuiStyleVar_FramePadding, ImVec2(std::stof(GetProperty("padding_x")), std::stof(GetProperty("padding_y"))));

          CherryGUI::InputText(Label.c_str(), buffer, sizeof(buffer));
          CherryGUI::PopStyleVar();
          *m_Value = std::string(buffer);
        } else {
          CherryGUI::Text("INVALID VALUE");
        }
      }

     private:
      std::string *m_Value;
    };
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline std::shared_ptr<Component> InputString(const std::string &label, std::string *value) {
      // Inline component
      auto anonymous_id = Application::GenerateUniqueID(label, value);
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing;
      } else {
        auto button = Application::CreateAnonymousComponent<Components::InputString>(
            Components::InputString(anonymous_id, label, value));
        button->Render();
        return button;
      }
    }

    inline std::shared_ptr<Component>
    InputString(const Cherry::Identifier &identifier, const std::string &label, std::string *value) {
      // Get the object if exist
      auto existing_keyval = Application::GetComponent(identifier);
      if (existing_keyval) {
        existing_keyval->RefreshContextProperties();
        existing_keyval->Render();
      } else {
        // Create the object if not exist
        auto new_keyval =
            Application::CreateComponent<Components::InputString>(Components::InputString(identifier, label, value));
        new_keyval->RefreshContextProperties();
        new_keyval->Render();
        return new_keyval;
      }
      return existing_keyval;
    }
  }  // namespace Kit

}  // namespace Cherry

#endif  // CHERRY_KIT_INPUT_STRING
