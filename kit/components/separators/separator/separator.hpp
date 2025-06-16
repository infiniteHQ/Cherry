#pragma once
#include "../../../../platform/engine/app/app.hpp"
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
    SetProperty("color", "theme:separator_color");
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
inline Component &Separator(const Identifier &identifier) {
  return CherryApp.PushComponent<Cherry::Components::Separator>(identifier);
}

inline Component &Separator() {
  return Cherry::Kit::Separator(Application::GenerateUniqueID("Separator"));
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_SEPARATOR
