#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ModalSimple
//

#ifndef CHERRY_KIT_MODAL_TITLE
#define CHERRY_KIT_MODAL_TITLE

namespace Cherry {
namespace Components {
class ModalSimple : public Component {
public:
  ModalSimple(const Cherry::Identifier &id, const std::string &title,
              bool *open, const std::function<void()> &callback)
      : Component(id), activated(open), render_callback(callback) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("padding_x", "6.0"); // TODO get the default theme
    SetProperty("padding_y", "6.0"); // TODO get the default theme

    // Informations
    SetProperty("title", title);

    if (activated) {
      SetProperty("activated", std::to_string(*activated));
    }
  }

  void Render() override {
    cached_title = GetProperty("title");
    padding_y = std::stof(GetProperty("padding_y"));
    padding_x = std::stof(GetProperty("padding_x"));

    if (!activated)
      return;

    if (*activated) {
      if (GetProperty("activated") == "false") {
        CherryGUI::CloseCurrentPopup();
        *activated = false;
        SetProperty("activated", std::to_string(*activated));
      }

      CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                              ImVec2(padding_x, padding_y));
      CherryGUI::OpenPopup(cached_title.c_str());

      ImVec2 main_window_size = CherryGUI::GetWindowSize();
      ImVec2 window_pos = CherryGUI::GetWindowPos();

      CherryGUI::SetNextWindowPos(
          ImVec2(window_pos.x + (main_window_size.x * 0.5f) - 300,
                 window_pos.y + 150));

      CherryGUI::SetNextWindowSize(ImVec2(600, 0), ImGuiCond_Always);
      // ImGuiWindowFlags_NoMove
      if (CherryGUI::BeginPopupModal(cached_title.c_str(), NULL,
                                     ImGuiWindowFlags_AlwaysAutoResize |
                                         ImGuiWindowFlags_NoScrollbar)) {
        if (render_callback) {
          render_callback();
        }
        CherryGUI::EndPopup();
      }

      CherryGUI::PopStyleVar();
    }
  }

private:
  std::function<void()> render_callback;
  ImVec4 cached_text_color;
  std::string cached_title;
  float font_scale;
  float padding_x;
  float padding_y;
  bool *activated;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &ModalSimple(
    const Identifier &identifier, const std::string &title, bool *activated,
    const std::function<void()> &render_callback = []() {}) {
  return CherryApp.PushComponent<Cherry::Components::ModalSimple>(
      identifier, title, activated, render_callback);
}

inline Component &ModalSimple(
    const std::string &title, bool *activated,
    const std::function<void()> &render_callback = []() {}) {
  return Cherry::Kit::ModalSimple(
      Application::GenerateUniqueID(title, activated, render_callback), title,
      activated, render_callback);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_MODAL_TITLE
