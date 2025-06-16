#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ImageHttp
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_IMAGE_HTTP
#define CHERRY_KIT_IMAGE_HTTP

namespace Cherry {
namespace Components {
#ifdef CHERRY_ENABLE_NET
class ImageHttp : public Component {
public:
  ImageHttp(const Cherry::Identifier &id, const std::string &image_http_path,
            float x_size, float y_size)
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
    SetProperty("image_http_path", image_http_path);

    // Data & User-level informations
    SetData("isChecked", "false");
  }

  void Render() override {
    CherryGUI::Image(
        Cherry::GetTexture(Cherry::GetHttpPath(GetProperty("image_http_path"))),
        ImVec2(std::stof(GetProperty("size_x")),
               std::stof(GetProperty("size_y"))));
  }
};
#else  // CHERRY_ENABLE_NET
class ImageHttp : public Component {
public:
  ImageHttp(const Cherry::Identifier &id, const std::string &image_http_path,
            float x_size, float y_size)
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
    SetProperty("image_http_path", image_http_path);

    // Data & User-level informations
    SetData("isChecked", "false");
  }

  void Render() override {
    CherryGUI::TextColored(
        Cherry::HexToRGBA("#FF3333FF"),
        "ERROR: Cannot display http(s) image without define CHERRY_ENABLE_NET");
  }
};
#endif // CHERRY_ENABLE_NET

} // namespace Components

// End-User API
namespace Kit {
inline Component &ImageHttp(const Identifier &identifier,
                            const std::string &image_http_path,
                            float x_size = 50.0f, float y_size = 50.0f) {
  return CherryApp.PushComponent<Cherry::Components::ImageHttp>(
      identifier, image_http_path, x_size, y_size);
}

inline Component &ImageHttp(const std::string &image_http_path,
                            float x_size = 50.0f, float y_size = 50.0f) {
  return Cherry::Kit::ImageHttp(Application::GenerateUniqueID(image_http_path,
                                                              x_size, y_size,
                                                              "ImageHttp"),
                                image_http_path, x_size, y_size);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_IMAGE_HTTP
