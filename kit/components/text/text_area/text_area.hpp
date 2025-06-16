#pragma once
#include "../../../../lib/imgui/misc/text_editors/text_editor.hpp"
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextArea
//

#ifndef CHERRY_KIT_TEXT_AREA
#define CHERRY_KIT_TEXT_AREA

namespace Cherry {
namespace Components {
class TextArea : public Component {
public:
  TextArea(const Cherry::Identifier &id, float *width, float *height,
           const std::string &buffer)
      : Component(id), m_Width(width), m_Height(height), m_TextEditor(buffer) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:text_area_color_text");
  }

  void Render() override {
    if (!m_Width || !m_Height) {
      return;
    }

    Cherry::PushFont("FiraCode");

    m_TextEditor.render();

    Cherry::PopFont();
  }

private:
  float *m_Width;
  float *m_Height;
  TextEditor m_TextEditor;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &TextArea(const Identifier &identifier, float *width,
                           float *height, const std::string &buffer) {
  return CherryApp.PushComponent<Cherry::Components::TextArea>(
      identifier, width, height, buffer);
}

inline Component &TextArea(float *width, float *height,
                           const std::string &buffer) {
  return Cherry::Kit::TextArea(
      Application::GenerateUniqueID(width, height, buffer, "TextArea"), width,
      height, buffer);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_AREA
