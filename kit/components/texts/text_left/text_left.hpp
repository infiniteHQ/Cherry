#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextLeft
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_LEFT
#define CHERRY_KIT_TEXT_LEFT

namespace Cherry {
namespace Components {
class TextLeft : public Component {
public:
  TextLeft(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#454545B2");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    std::string text = GetProperty("label").c_str();
    CherryGUI::TextWrapped(text.c_str());
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TextLeft(const Identifier &identifier,
                           const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextLeft>(identifier,
                                                               label);
}

inline Component &TextLeft(const std::string &label) {
  return Cherry::Kit::TextLeft(Application::GenerateUniqueID(label), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_LEFT
