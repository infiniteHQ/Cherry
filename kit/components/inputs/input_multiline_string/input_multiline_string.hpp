#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputMultilineString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_MULTILINE_STRING
#define CHERRY_KIT_INPUT_MULTILINE_STRING

namespace Cherry {
namespace Components {
class InputMultilineString : public Component {
public:
  InputMultilineString(const Cherry::Identifier &id, const std::string &label,
                       std::string *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#454545B2");

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

      // TODO remplace 16 by the number of lines we want to show
      CherryGUI::InputTextMultiline(
          Label.c_str(), buffer, sizeof(buffer),
          ImVec2(-FLT_MIN, CherryGUI::GetTextLineHeight() * 16));
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
inline Component &InputMultilineString(const Identifier &identifier,
                                       const std::string &label,
                                       std::string *value) {
  return CherryApp.PushComponent<Cherry::Components::InputMultilineString>(
      identifier, label, value);
}

inline Component &InputMultilineString(const std::string &label,
                                       std::string *value) {
  return Cherry::Kit::InputMultilineString(
      Application::GenerateUniqueID(label, value), label, value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_INPUT_MULTILINE_STRING
