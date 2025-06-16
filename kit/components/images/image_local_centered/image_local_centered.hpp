#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ImageLocalCentered
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_IMAGE_LOCAL_CENTERED
#define CHERRY_KIT_IMAGE_LOCAL_CENTERED

namespace Cherry {
namespace Components {
class ImageLocalCentered : public Component {
public:
  ImageLocalCentered(const Cherry::Identifier &id,
                     const std::string &image_path, float x_size, float y_size)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "theme:image_color_border");
    SetProperty("color_border_hovered", "theme:image_color_border_hovered");
    SetProperty("color_border_clicked", "theme:image_color_border_clicked");
    SetProperty("color_bg", "theme:image_color_bg");
    SetProperty("color_bg_hovered", "theme:image_color_bg_hovered");
    SetProperty("color_bg_clicked", "theme:image_color_bg_clicked");

    // Sizes
    SetProperty("size_x", std::to_string(x_size));
    SetProperty("size_y", std::to_string(y_size));

    // Informations
    SetProperty("image_path", image_path);

    // Data & User-level informations
    SetData("isChecked", "false");
  }

  void Render() override {
    ImVec2 child_avail = CherryGUI::GetContentRegionAvail();
    float posX = (child_avail.x - std::stof(GetProperty("size_x"))) * 0.5f;

    if (posX > 0)
      CherryGUI::SetCursorPosX(posX);

    CherryGUI::Image(Cherry::GetTexture(GetProperty("image_path")),
                     ImVec2(std::stof(GetProperty("size_x")),
                            std::stof(GetProperty("size_y"))));
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &ImageLocalCentered(const Identifier &identifier,
                                     const std::string &image_path,
                                     float x_size = 50.0f,
                                     float y_size = 50.0f) {
  return CherryApp.PushComponent<Cherry::Components::ImageLocalCentered>(
      identifier, image_path, x_size, y_size);
}

inline Component &ImageLocalCentered(const std::string &image_path,
                                     float x_size = 50.0f,
                                     float y_size = 50.0f) {
  return Cherry::Kit::ImageLocalCentered(
      Application::GenerateUniqueID(image_path, x_size, y_size,
                                    "ImageLocalCentered"),
      image_path, x_size, y_size);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_IMAGE_LOCAL_CENTERED
