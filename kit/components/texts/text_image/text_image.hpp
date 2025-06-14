#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_IMAGE
#define CHERRY_KIT_TEXT_IMAGE

namespace Cherry {
namespace Components {
class TextImage : public Component {
public:
  TextImage(const Cherry::Identifier &id, const std::string &label,
            const std::string &image_path)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#454545B2");

    // Informations
    SetProperty("label", label);
    SetProperty("image_path", image_path);
  }

  void Render() override {
    std::string text = GetProperty("label").c_str();

    ImTextureID texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("image_path"));
    CherryGUI::Image(texture, ImVec2(15, 15));
    CherryGUI::SameLine();
    CherryGUI::Text(text.c_str());
  }
};
} // namespace Components

// End-User API
namespace Kit {

inline Component &TextImage(const Identifier &identifier,
                            const std::string &label,
                            const std::string &image_path) {
  return CherryApp.PushComponent<Cherry::Components::TextImage>(
      identifier, label, image_path);
}

inline Component &TextImage(const std::string &label,
                            const std::string &image_path) {
  return Cherry::Kit::TextImage(
      Application::GenerateUniqueID(label, image_path), label, image_path);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_IMAGE
