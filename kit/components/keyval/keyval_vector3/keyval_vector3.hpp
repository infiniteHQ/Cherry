#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValVector3
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_VECTOR3
#define CHERRY_KIT_KEYVAL_VECTOR3

namespace Cherry {
namespace Components {
class KeyValVector3 : public Component {
public:
  KeyValVector3(const Cherry::Identifier &id, const std::string &label,
                float *x_value, float *y_value, float *z_value)
      : Component(id), m_XValue(x_value), m_YValue(y_value), m_ZValue(z_value) {
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

        if (m_XValue && m_YValue && m_ZValue) {

          float inputWidth = 100.0f;
          float colorBoxWidth = 20.0f;
          float spacing = 5.0f;
          float elementWidth = colorBoxWidth + spacing + inputWidth;
          float availableWidth = CherryGUI::GetContentRegionAvail().x;

          int elementsPerRow = (availableWidth >= 3 * elementWidth)   ? 3
                               : (availableWidth >= 2 * elementWidth) ? 2
                                                                      : 1;

          CherryGUI::PushItemWidth(inputWidth);

          for (int i = 0; i < 3; ++i) {
            ImVec2 pos = CherryGUI::GetCursorScreenPos();
            const char *texturePath;
            float *value;
            std::string id;

            switch (i) {
            case 0:
              texturePath = "resources/base/x.png";
              value = m_XValue;
              id = Label + "###1" + identifier;
              break;
            case 1:
              texturePath = "resources/base/y.png";
              value = m_YValue;
              id = Label + "###2" + identifier;
              break;
            case 2:
              texturePath = "resources/base/z.png";
              value = m_ZValue;
              id = Label + "###3" + identifier;
              break;
            }

            CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 1.0f);
            CherryGUI::Image(Cherry::GetTexture(Cherry::GetPath(texturePath)),
                             ImVec2(23, 23));
            CherryGUI::SetCursorScreenPos(
                ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
            CherryGUI::InputFloat(id.c_str(), value);

            if ((i + 1) % elementsPerRow != 0 && i < 2) {
              CherryGUI::SameLine();
            }
          }

          CherryGUI::PopItemWidth();
        } else {
          CherryGUI::Text("INVALID VALUE");
        }
      }
    }
  }

private:
  float *m_XValue;
  float *m_YValue;
  float *m_ZValue;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValVector3(const Identifier &identifier,
                                const std::string &label, float *x_value,
                                float *y_value, float *z_value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValVector3>(
      identifier, label, x_value, y_value, z_value);
}

inline Component &KeyValVector3(const std::string &label, float *x_value,
                                float *y_value, float *z_value) {
  return Cherry::Kit::KeyValVector3(
      Application::GenerateUniqueID(label, x_value, y_value), label, x_value,
      y_value, z_value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_VECTOR3
