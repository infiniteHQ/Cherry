#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextWrapped
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_WRAPPED
#define CHERRY_KIT_TEXT_WRAPPED

namespace Cherry {
namespace Components {
class TextWrapped : public Component {
public:
  TextWrapped(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:text_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    auto cached_text_color = Cherry::HexToRGBA(GetProperty("color_text"));
    CherryGUI::PushStyleColor(ImGuiCol_Text, cached_text_color);
    CherryGUI::TextWrapped(GetProperty("label").c_str());
    CherryGUI::PopStyleColor();
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TextWrapped(const Identifier &identifier,
                              const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextWrapped>(identifier,
                                                                  label);
}

inline Component &TextWrapped(const std::string &label) {
  return Cherry::Kit::TextWrapped(
      Application::GenerateUniqueID(label, "TextWrapped"), label);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_WRAPPED
