#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// CheckboxText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_CHECKBOX_TEXT
#define CHERRY_KIT_CHECKBOX_TEXT

namespace Cherry {
namespace Components {
class CheckboxText : public Component {
public:
  CheckboxText(const Cherry::Identifier &id, const std::string &label,
               bool *checked)
      : Component(id), m_Checked(checked) {
    if (m_Checked == NULL) {
      m_CheckedDefault = false;
      m_Checked = &m_CheckedDefault;
    }

    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "#454545B2");
    SetProperty("color_border_hovered", "#454545B2");
    SetProperty("color_border_clicked", "#454545B2");
    SetProperty("color_bg", "#242424FF");
    SetProperty("color_bg_hovered", "#343434FF");
    SetProperty("color_bg_clicked", "#444444FF");

    // Sizes
    SetProperty("size_x", "6");
    SetProperty("size_y", "6");

    // Informations
    SetProperty("label", label);

    // Data & User-level informations
    SetData("isChecked", "false");
  }

  void Render() override {
    CherryGUI::Checkbox(GetProperty("label").c_str(), m_Checked);

    if (m_Checked) {
      SetData("isChecked", "true");
    } else {
      SetData("isChecked", "false");
    }
  }

private:
  void UpdateLastClickTime() {
    std::string m_LastClickTime;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    m_LastClickTime = std::ctime(&now_c);
    m_LastClickTime.erase(m_LastClickTime.length() - 1);
    SetData("lastClicked", m_LastClickTime);
  }
  bool *m_Checked;
  bool m_CheckedDefault;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &CheckboxText(const Identifier &identifier,
                               const std::string &label, bool *checked) {
  return CherryApp.PushComponent<Cherry::Components::CheckboxText>(
      identifier, label, checked);
}

inline Component &CheckboxText(const std::string &label, bool *checked) {
  return Cherry::Kit::CheckboxText(
      Application::GenerateUniqueID(label, checked), label, checked);
}
} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_CHECKBOX_TEXT
