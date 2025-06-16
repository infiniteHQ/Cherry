#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// HeaderImageText
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_HEADER_IMAGE_TEXT
#define CHERRY_KIT_HEADER_IMAGE_TEXT

namespace Cherry {
namespace Components {
class HeaderImageText : public Component {
public:
  HeaderImageText(const Cherry::Identifier &id, const std::string &label,
                  const std::string &image_path,
                  const std::function<void()> &render_content)
      : Component(id), m_RenderContent(render_content) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_border", "theme:header_color_border");
    SetProperty("color_border_hovered", "theme:header_color_border_hovered");
    SetProperty("color_border_clicked", "theme:header_color_border_clicked");
    SetProperty("color_bg", "theme:header_color_bg");
    SetProperty("color_bg_hovered", "theme:header_color_bg_hovered");
    SetProperty("color_bg_clicked", "theme:header_color_bg_clicked");

    // Sizes
    SetProperty("size_x", "theme:header_size_x");
    SetProperty("size_y", "theme:header_size_y");

    // Images
    SetProperty("image_path", image_path);

    // Informations
    SetProperty("label", label);

    // Data & User-level informations
    SetData("lastClicked", "never");
    SetData("isClicked", "false");
  }

  void Render() override {
    CherryGUI::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_Button,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered,
                              HexToRGBA(GetProperty("color_bg_hovered")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonActive,
                              HexToRGBA(GetProperty("color_bg_clicked")));

    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    ImGuiStyle &style = CherryGUI::GetStyle();

    float texture_size = CherryGUI::GetFontSize();
    float padding = style.ItemInnerSpacing.x;

    CherryGUI::BeginGroup();

    bool isOpened = GetData("isOpened") == "true";

    if (CherryGUI::ImageSizeButtonWithText(
            Cherry::GetTexture(GetProperty("image_path")), 700.0f,
            Label.c_str(), ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1),
            -1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
      SetData("isOpened", isOpened ? "false" : "true");
    }

    ImVec2 arrow_pos =
        ImVec2(CherryGUI::GetItemRectMax().x - style.FramePadding.x -
                   CherryGUI::GetFontSize(),
               CherryGUI::GetItemRectMin().y + style.FramePadding.y + 3.0f);
    CherryGUI::RenderArrow(CherryGUI::GetWindowDrawList(), arrow_pos,
                           CherryGUI::GetColorU32(ImGuiCol_Text),
                           GetData("isOpened") == "true" ? ImGuiDir_Down
                                                         : ImGuiDir_Right);
    CherryGUI::EndGroup();

    CherryGUI::PopStyleColor(4);
    CherryGUI::PopStyleVar();

    if (GetData("isOpened") == "true") {
      CherryID::UpgradeIncrementorLevel();
      if (m_RenderContent) {
        m_RenderContent();
      }
      CherryID::DowngradeIncrementorLevel();
    }
  }

private:
  void UpdateLastClickTime() {
    std::string m_LastClickTime;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    m_LastClickTime = std::ctime(&now_c);
    m_LastClickTime.erase(m_LastClickTime.length() - 1);
    SetData("lastClicked", m_LastClickTime);
  }

  std::function<void()> m_RenderContent;
};
} // namespace Components

// End-User API
namespace Kit {

inline Component &HeaderImageText(
    const Identifier &identifier, const std::string &label,
    const std::string &image_path,
    const std::function<void()> &render_content = []() {}) {
  return CherryApp.PushComponent<Cherry::Components::HeaderImageText>(
      identifier, label, image_path, render_content);
}

inline Component &HeaderImageText(
    const std::string &label, const std::string &image_path,
    const std::function<void()> &render_content = []() {}) {
  return Cherry::Kit::HeaderImageText(
      Application::GenerateUniqueID(label, image_path, render_content,
                                    "HeaderImageText"),
      label, image_path, render_content);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_HEADER_IMAGE_TEXT
