#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValCustom
//

#ifndef CHERRY_KIT_KEYVAL_CUSTOM
#define CHERRY_KIT_KEYVAL_CUSTOM

namespace Cherry {
namespace Components {
class KeyValCustom : public Component {
public:
  KeyValCustom(const Cherry::Identifier &id, const std::string &label,
               const std::function<void()> &renderCallback)
      : Component(id), m_RenderCallback(renderCallback) {
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

        if (m_RenderCallback) {
          m_RenderCallback();
        } else {
          CherryGUI::Text("INVALID VALUE");
        }
      }
    }
  }

private:
  std::function<void()> m_RenderCallback;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValCustom(const Identifier &identifier,
                               const std::string &label,
                               const std::function<void()> &renderCallback) {
  return CherryApp.PushComponent<Cherry::Components::KeyValCustom>(
      identifier, label, renderCallback);
}

inline Component &KeyValCustom(const std::string &label,
                               const std::function<void()> &renderCallback) {
  return Cherry::Kit::KeyValCustom(
      Application::GenerateUniqueID(label, renderCallback, "KeyValCustom"),
      label, renderCallback);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_CUSTOM
