#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// KeyValVector4
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_KEYVAL_VECTOR4
#define CHERRY_KIT_KEYVAL_VECTOR4

namespace Cherry {
namespace Components {
class KeyValVector4 : public Component {
public:
  KeyValVector4(const Cherry::Identifier &id, const std::string &label,
                float *x_value, float *y_value, float *z_value, float *w_value)
      : Component(id), m_XValue(x_value), m_YValue(y_value), m_ZValue(z_value),
        m_WValue(w_value) {
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

        if (m_XValue && m_YValue && m_ZValue && m_WValue) {

          float inputWidth = 100.0f;
          float colorBoxWidth = 20.0f;
          float spacing = 5.0f;
          float elementWidth = colorBoxWidth + spacing + inputWidth;
          float availableWidth = CherryGUI::GetContentRegionAvail().x;

          int elementsPerRow = (availableWidth >= 4 * elementWidth)   ? 4
                               : (availableWidth >= 3 * elementWidth) ? 3
                               : (availableWidth >= 2 * elementWidth) ? 2
                                                                      : 1;

          CherryGUI::PushItemWidth(inputWidth);

          for (int i = 0; i < 4; ++i) {
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
            case 3:
              texturePath = "resources/base/w.png";
              value = m_WValue;
              id = Label + "###4" + identifier;
              break;
            }

            CherryGUI::SetCursorPosY(CherryGUI::GetCursorPosY() + 1.0f);
            CherryGUI::Image(Cherry::GetTexture(Cherry::GetPath(texturePath)),
                             ImVec2(23, 23));
            CherryGUI::SetCursorScreenPos(
                ImVec2(pos.x + colorBoxWidth + spacing, pos.y));
            CherryGUI::InputFloat(id.c_str(), value);

            if ((i + 1) % elementsPerRow != 0 && i < 3) {
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
  float *m_WValue;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValVector4(const Identifier &identifier,
                                const std::string &label, float *x_value,
                                float *y_value, float *z_value,
                                float *w_value) {
  return CherryApp.PushComponent<Cherry::Components::KeyValVector4>(
      identifier, label, x_value, y_value, z_value, w_value);
}

inline Component &KeyValVector4(const std::string &label, float *x_value,
                                float *y_value, float *z_value,
                                float *w_value) {
  return Cherry::Kit::KeyValVector4(
      Application::GenerateUniqueID(label, x_value, y_value, "KeyValVector4"),
      label, x_value, y_value, z_value, w_value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_VECTOR4
