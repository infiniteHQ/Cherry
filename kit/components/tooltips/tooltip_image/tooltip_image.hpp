#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TooltipImage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TOOLTIP_IMAGE
#define CHERRY_KIT_TOOLTIP_IMAGE

namespace Cherry {
namespace Components {
class TooltipImage : public Component {
public:
  TooltipImage(const Cherry::Identifier &id, const std::string &image_path,
               const std::string &description)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "theme:tooltip_color_border");
    SetProperty("color_border_hovered", "theme:tooltip_color_border_hovered");
    SetProperty("color_border_clicked", "theme:tooltip_color_border_clicked");
    SetProperty("color_bg", "theme:tooltip_color_bg");
    SetProperty("color_bg_hovered", "theme:tooltip_color_bg_hovered");
    SetProperty("color_bg_clicked", "theme:tooltip_color_bg_clicked");

    // Informations
    SetProperty("image_path", image_path);
    SetProperty("description", description);
  }

  void Render() override {
    ImTextureID texture =
        Application::Get().GetCurrentRenderedWindow()->get_texture(
            GetProperty("image_path"));
    CherryGUI::Image(texture, ImVec2(15, 15));

    if (CherryGUI::IsItemHovered()) {
      CherryGUI::BeginTooltip();
      CherryGUI::PushTextWrapPos(CherryGUI::GetFontSize() * 35.0f);
      CherryGUI::TextUnformatted(GetProperty("description").c_str());
      CherryGUI::PopTextWrapPos();
      CherryGUI::EndTooltip();
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TooltipImage(const Identifier &identifier,
                               const std::string &image_path,
                               const std::string &description) {
  return CherryApp.PushComponent<Cherry::Components::TooltipImage>(
      identifier, image_path, description);
}

inline Component &TooltipImage(const std::string &image_path,
                               const std::string &description) {
  return Cherry::Kit::TooltipImage(
      Application::GenerateUniqueID(image_path, description, "TooltipImage"),
      image_path, description);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_TOOLTIP_IMAGE
