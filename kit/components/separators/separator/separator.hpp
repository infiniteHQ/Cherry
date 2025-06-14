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
inline Component &Separator(const Cherry::Identifier &identifier) {
  // Get the object if exist
  auto existing_title = Application::GetComponent(identifier);
  if (existing_title.GetIdentifier().string() != "undefined") {
    existing_title.RenderWrapper();
  } else {
    // Create the object if not exist
    auto new_title = Application::CreateComponent<Components::Separator>(
        Components::Separator(identifier));
    new_title->RenderWrapper();
    return *new_title;
  }
  return existing_title;
}

inline Component &Separator() {
  // Inline component
  auto anonymous_id = Application::GenerateUniqueID("separator");
  return Cherry::Kit::Separator(anonymous_id);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_SEPARATOR
