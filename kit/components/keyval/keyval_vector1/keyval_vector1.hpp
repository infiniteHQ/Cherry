#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValVector1
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_VECTOR1FLOAT
#define CHERRY_KIT_KEYVAL_VECTOR1FLOAT

namespace Cherry {
namespace Components {
class KeyValVector1 : public Component {
public:
  KeyValVector1(const Cherry::Identifier &id, const std::string &label,
                float *x_value)
      : Component(id), m_XValue(x_value) {
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

        if (m_XValue) {
          float inputWidth = 100.0f;
          float colorBoxWidth = 20.0f;
          float spacing = 5.0f;

          CherryGUI::PushItemWidth(inputWidth);

          ImVec2 pos = CherryGUI::GetCursorScreenPos();
          CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 1.0f);
          CherryGUI::Image(
              Cherry::GetTexture(Cherry::GetPath("resources/base/x.png")),
              ImVec2(23, 23));
          CherryGUI::SetCursorScreenPos(
              ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
          CherryGUI::InputFloat((Label + "###X" + identifier).c_str(),
                                m_XValue);

          CherryGUI::PopItemWidth();
        } else {
          CherryGUI::Text("INVALID VALUE");
        }
      }
    }
  }

private:
  float *m_XValue;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValVector1(const Identifier &identifier,
                                const std::string &label, float *x_value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValVector1>(
      identifier, label, x_value);
}

inline Component &KeyValVector1(const std::string &label, float *x_value) {
  return Cherry::Kit::KeyValVector1(
      Application::GenerateUniqueID(label, x_value), label, x_value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_VECTOR1FLOAT
