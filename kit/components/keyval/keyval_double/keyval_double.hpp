#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// KeyValDouble
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_DOUBLE
#define CHERRY_KIT_KEYVAL_DOUBLE

namespace Cherry {
namespace Components {
class KeyValDouble : public Component {
public:
  KeyValDouble(const Cherry::Identifier &id, const std::string &label,
               double *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:keyval_color_text");

    // Behavior
    SetProperty("step", "1");
    SetProperty("step_fast", "10");

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
          CherryGUI::InputDouble(Label.c_str(), m_Value);
        } else {
          CherryGUI::Text("INVALID VALUE");
        }
      }
    }
  }

private:
  double *m_Value;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValDouble(const Identifier &identifier,
                               const std::string &label, double *value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValDouble>(
      identifier, label, value);
}

inline Component &KeyValDouble(const std::string &label, double *value) {
  return Cherry::Kit::KeyValDouble(
      Application::GenerateUniqueID(label, value, "KeyValDouble"), label,
      value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_DOUBLE
