#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// BulletText
// Summary : A big bullet.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BULLET_TEXT
#define CHERRY_KIT_BULLET_TEXT

namespace Cherry {
namespace Components {
class BulletText : public Component {
public:
  BulletText(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:button_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::BulletText(GetProperty("label").c_str());
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &BulletText(const Identifier &identifier,
                             const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::BulletText>(identifier,
                                                                 label);
}

inline Component &BulletText(const std::string &label) {
  return Cherry::Kit::BulletText(
      Application::GenerateUniqueID(label, "BulletText"), label);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BULLET_TEXT
