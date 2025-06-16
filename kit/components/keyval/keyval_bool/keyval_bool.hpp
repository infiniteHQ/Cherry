#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValBool
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_BOOL
#define CHERRY_KIT_KEYVAL_BOOL

namespace Cherry {
namespace Components {
class KeyValBool : public Component {
public:
  KeyValBool(const Cherry::Identifier &id, const std::string &label,
             bool *value)
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
          CherryGUI::Checkbox(Label.c_str(), m_Value);
        } else {
          CherryGUI::Text("INVALID VALUE");
        }
      }
    }
  }

private:
  bool *m_Value;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValBool(const Identifier &identifier,
                             const std::string &label, bool *value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValBool>(identifier,
                                                                 label, value);
}

inline Component &KeyValBool(const std::string &label, bool *value) {
  return Cherry::Kit::KeyValBool(
      Application::GenerateUniqueID(label, value, "KeyValBool"), label, value);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_BOOL
