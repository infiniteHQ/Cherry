#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// Separator
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_SEPARATOR
#define CHERRY_KIT_SEPARATOR

namespace Cherry {
namespace Components {
class Separator : public Component {
public:
  Separator(const Cherry::Identifier &id) : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color", "#424242");
  }

  void Render() override {
    CherryGUI::PushStyleColor(ImGuiCol_Separator,
                              Cherry::HexToRGBA(GetProperty("color")));
    CherryGUI::Separator();
    CherryGUI::PopStyleColor();
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline std::shared_ptr<Component> Separator() {
  // Inline component
  auto anonymous_id = Application::GenerateUniqueID("separator");
  auto existing = Application::GetAnonymousComponent(anonymous_id);
  if (existing) {
    existing->RenderWrapper();
    return existing;
  } else {
    auto button = Application::CreateAnonymousComponent<Components::Separator>(
        Components::Separator(anonymous_id));
    button->RenderWrapper();
    return button;
  }
}

inline std::shared_ptr<Component>
Separator(const Cherry::Identifier &identifier) {
  // Get the object if exist
  auto existing_title = Application::GetComponent(identifier);
  if (existing_title) {
    existing_title->RenderWrapper();
  } else {
    // Create the object if not exist
    auto new_title = Application::CreateComponent<Components::Separator>(
        Components::Separator(identifier));
    new_title->RenderWrapper();
    return new_title;
  }
  return existing_title;
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_SEPARATOR
