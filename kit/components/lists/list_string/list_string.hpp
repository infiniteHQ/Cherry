#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ListString
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_LIST_STRING
#define CHERRY_KIT_LIST_STRING

namespace Cherry {
namespace Components {
class ListString : public Component {
public:
  ListString(const Cherry::Identifier &id, const std::string &label,
             const std::vector<std::string> &values, int default_selected)
      : Component(id), m_Values(values) {
    // Identifier
    SetIdentifier(id);

    SetProperty("label", label);
    SetProperty("default_selected", std::to_string(default_selected));
    SetProperty("current_selected", std::to_string(default_selected));
  }

  void Render() override {
    std::string identifier = GetIdentifier().string();
    std::string Label;

    if (!identifier.empty()) {
      Label += Label + "####" + identifier;
    }

    int current_selected = std::stoi(GetProperty("current_selected"));

    if (CherryGUI::BeginListBox(Label.c_str())) {
      for (int n = 0; n < m_Values.size(); n++) {
        const bool is_selected = (current_selected == n);
        if (CherryGUI::Selectable(m_Values[n].c_str(), is_selected)) {
          current_selected = n;
          SetProperty("current_selected", std::to_string(current_selected));
        }

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected)
          CherryGUI::SetItemDefaultFocus();
      }
      CherryGUI::EndListBox();
    }
  }

private:
  std::vector<std::string> m_Values;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &ListString(const Identifier &identifier,
                             const std::string &label,
                             const std::vector<std::string> &values,
                             int default_selected = 0) {
  return CherryApp.PushComponent<Cherry::Components::ListString>(
      identifier, label, values, default_selected);
}

inline Component &ListString(const std::string &label,
                             const std::vector<std::string> &values,
                             int default_selected = 0) {
  return Cherry::Kit::ListString(
      Application::GenerateUniqueID(label, values, default_selected), label,
      values, default_selected);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_LIST_STRING
