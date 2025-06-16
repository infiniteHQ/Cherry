#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// BulletMultiText
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BULLET_MULTI_TEXT
#define CHERRY_KIT_BULLET_MULTI_TEXT

namespace Cherry {
namespace Components {
class BulletMultiText : public Component {
public:
  BulletMultiText(const Cherry::Identifier &id,
                  const std::vector<std::string> &labels)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:button_color_text");

    // Informations
    std::string label;
    bool first_label = true;
    for (auto string_label : labels) {
      std::string extension = first_label == false ? "\n" : "";
      label += string_label + extension;
      first_label = false;
    }

    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::BulletText(GetProperty("label").c_str());
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &BulletMultiText(const Identifier &identifier,
                                  const std::vector<std::string> &labels) {
  return CherryApp.PushComponent<Cherry::Components::BulletMultiText>(
      identifier, labels);
}

inline Component &BulletMultiText(const std::vector<std::string> &labels) {
  return Cherry::Kit::BulletMultiText(
      Application::GenerateUniqueID(labels, "BulletMultiText"), labels);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BULLET_MULTI_TEXT
