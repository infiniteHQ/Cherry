#pragma once
#include "../../../../platform/engine/app.hpp"
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
    SetProperty("color_border", "#454545B2");
    SetProperty("color_border_hovered", "#454545B2");
    SetProperty("color_border_clicked", "#454545B2");
    SetProperty("color_bg", "#242424FF");
    SetProperty("color_bg_hovered", "#343434FF");
    SetProperty("color_bg_clicked", "#444444FF");

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
      Application::GenerateUniqueID(image_path, description), image_path,
      description);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_TOOLTIP_IMAGE
