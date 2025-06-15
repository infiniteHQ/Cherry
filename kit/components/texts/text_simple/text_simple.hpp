#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextSimple
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_SIMPLE
#define CHERRY_KIT_TEXT_SIMPLE

namespace Cherry {
namespace Components {
class TextSimple : public Component {
public:
  TextSimple(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#FFFFFFFF");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                           GetProperty("label").c_str());

    auto parent = Cherry::GetParent();
    if (parent) {
      CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                             parent->GetProperty("label").c_str());
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TextSimple(const Identifier &identifier,
                             const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextSimple>(identifier,
                                                                 label);
}

inline Component &TextSimple(const std::string &label) {
  return Cherry::Kit::TextSimple(
      Application::GenerateUniqueID(label + "TextSimple"), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_SIMPLE
