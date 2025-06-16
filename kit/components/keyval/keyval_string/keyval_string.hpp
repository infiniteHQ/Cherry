#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

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
  KeyValString(const Cherry::Identifier &id, const std::string &label,
               std::string *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:keyval_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    auto parent = Cherry::GetParent();
    if (parent) {
      if (std::stoi(parent->GetData("renderedColumn")) == 0) {
        CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                   IM_COL32(100, 100, 100, 40));
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

        CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                   IM_COL32(100, 100, 100, 40));

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
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValString(const Identifier &identifier,
                               const std::string &label, std::string *value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValString>(
      identifier, label, value);
}

inline Component &KeyValString(const std::string &label, std::string *value) {
  return Cherry::Kit::KeyValString(
      Application::GenerateUniqueID(label, value, "KeyValString"), label,
      value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_STRING
