#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BulletButton
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BULLET_BUTTON
#define CHERRY_KIT_BULLET_BUTTON

namespace Cherry {
namespace Components {
class BulletButton : public Component {
public:
  BulletButton(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:button_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::Bullet();
    CherryGUI::Button(GetProperty("label").c_str());
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &BulletButton(const Identifier &identifier,
                               const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::BulletButton>(identifier,
                                                                   label);
}

inline Component &BulletButton(const std::string &label) {
  return Cherry::Kit::BulletButton(
      Application::GenerateUniqueID(label, "BulletButton"), label);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BULLET_BUTTON
