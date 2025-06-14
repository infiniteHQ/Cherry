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
  InputString(const Cherry::Identifier &id, const std::string &label,
              std::string *value)
      : Component(id), m_Value(value) {
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

      CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                              ImVec2(std::stof(GetProperty("padding_x")),
                                     std::stof(GetProperty("padding_y"))));

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
} // namespace Components

// End-User API
namespace Kit {
inline Component &InputString(const Identifier &identifier,
                              const std::string &label, std::string *value) {
  return CherryApp.PushComponent<Cherry::Components::InputString>(identifier,
                                                                  label, value);
}

inline Component &InputString(const std::string &label, std::string *value) {
  return Cherry::Kit::InputString(Application::GenerateUniqueID(label, value),
                                  label, value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_INPUT_STRING
