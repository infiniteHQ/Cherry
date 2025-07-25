#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// ListCustom
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_LIST_IMAGE_COMBO
#define CHERRY_KIT_LIST_IMAGE_COMBO

namespace Cherry {
namespace Components {
class ListCustom : public Component {
public:
  ListCustom(const Cherry::Identifier &id, const std::string &label,
             const std::vector<Component> &values, int default_selected)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    for (auto element : values) {
      m_Values.push_back(CherryApp.GetComponentPtr(element.GetIdentifier()));
    }

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
        if (CherryGUI::Selectable(
                std::string("####" + identifier + std::to_string(n)).c_str(),
                is_selected)) {
          current_selected = n;
          SetProperty("current_selected", std::to_string(current_selected));
        }

        CherryGUI::SameLine();
        CherryApp.PushParentComponent(
            CherryApp.GetComponentPtr(this->GetIdentifier()));
        m_Values[n]->Render();
        CherryApp.PopParentComponent();

        // Set the initial focus when opening the combo (scrolling + keyboard
        // navigation focus)
        if (is_selected)
          CherryGUI::SetItemDefaultFocus();
      }
      CherryGUI::EndListBox();
    }
  }

private:
  std::vector<std::shared_ptr<Component>> m_Values;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &ListCustom(const Identifier &identifier,
                             const std::string &label,
                             const std::vector<Component> &values,
                             int default_selected = 0) {
  return CherryApp.PushComponent<Cherry::Components::ListCustom>(
      identifier, label, values, default_selected);
}

inline Component &ListCustom(const std::string &label,
                             const std::vector<Component> &values,
                             int default_selected = 0) {
  return Cherry::Kit::ListCustom(Application::GenerateUniqueID(label, values,
                                                               default_selected,
                                                               "ListCustom"),
                                 label, values, default_selected);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_LIST_IMAGE_COMBO