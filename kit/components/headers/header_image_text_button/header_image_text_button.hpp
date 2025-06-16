#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// HeaderImageTextButton
// Summary : A simple button with a simple text, quite simple !
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_HEADER_IMAGE_TEXT_BUTTON
#define CHERRY_KIT_HEADER_IMAGE_TEXT_BUTTON

namespace Cherry {
namespace Components {
class HeaderImageTextButton : public Component {
public:
  HeaderImageTextButton(const Cherry::Identifier &id, const std::string &label,
                        const std::string &image_path,
                        const std::function<void()> &render_content)
      : Component(id), m_RenderContent(render_content) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("button_color_border", "theme:header_button_color_border");
    SetProperty("button_color_border_hovered",
                "theme:header_button_color_border_hovered");
    SetProperty("button_color_border_clicked",
                "theme:header_button_color_border_clicked");
    SetProperty("button_color_bg", "theme:header_button_color_bg");
    SetProperty("button_color_bg_hovered",
                "theme:header_button_color_bg_hovered");
    SetProperty("button_color_bg_clicked",
                "theme:header_button_color_bg_clicked");
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
    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }
    CherryGUI::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.0f, 0.5f));

    ImGuiStyle &style = CherryGUI::GetStyle();
    float texture_size = CherryGUI::GetFontSize();
    float padding = style.ItemInnerSpacing.x;

    CherryGUI::BeginGroup();

    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_Button,
                              HexToRGBA(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered,
                              HexToRGBA(GetProperty("color_bg_hovered")));
    CherryGUI::PushStyleColor(ImGuiCol_ButtonActive,
                              HexToRGBA(GetProperty("color_bg_clicked")));

    std::string button_label = Label + "_button";

    bool isOpened = GetData("isOpened") == "true";

    if (CherryGUI::ImageSizeButtonWithText(
            Cherry::GetTexture(GetProperty("image_path")),
            std::stof(GetProperty("size_x")), button_label.c_str(),
            ImVec2(-FLT_MIN, 0.0f), ImVec2(0, 0), ImVec2(1, 1), -1,
            ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1))) {
      SetData("isOpened", isOpened ? "false" : "true");
    }

    CherryGUI::PopStyleColor(4);

    ImVec2 arrow_pos =
        ImVec2(CherryGUI::GetItemRectMax().x - style.FramePadding.x -
                   CherryGUI::GetFontSize(),
               CherryGUI::GetItemRectMin().y + style.FramePadding.y + 3.0f);
    CherryGUI::RenderArrow(CherryGUI::GetWindowDrawList(), arrow_pos,
                           CherryGUI::GetColorU32(ImGuiCol_Text),
                           GetData("isOpened") == "true" ? ImGuiDir_Down
                                                         : ImGuiDir_Right);

    ImVec2 button_size = CherryGUI::GetItemRectSize();
    ImVec2 button_pos = CherryGUI::GetItemRectMin();
    ImVec2 content_pos;
    content_pos.x = button_pos.x + button_size.x - texture_size - 100;
    content_pos.y = button_pos.y;

    CherryGUI::SetCursorScreenPos(content_pos);
    ImVec2 child_size = ImVec2(100, button_size.y);

    CherryGUI::PushStyleColor(ImGuiCol_ChildBg,
                              Cherry::HexToImU32("#00000000"));

    std::string child_label = "###child" + identifier;
    CherryGUI::BeginChild(child_label.c_str(), child_size, false,
                          ImGuiWindowFlags_NoMove |
                              ImGuiWindowFlags_NoScrollbar |
                              ImGuiWindowFlags_NoScrollWithMouse);

    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              HexToRGBA(GetProperty("button_color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_Button,
                              HexToRGBA(GetProperty("button_color_border")));
    CherryGUI::PushStyleColor(
        ImGuiCol_ButtonHovered,
        HexToRGBA(GetProperty("button_color_bg_hovered")));
    CherryGUI::PushStyleColor(
        ImGuiCol_ButtonActive,
        HexToRGBA(GetProperty("button_color_bg_clicked")));

    CherryGUI::PopStyleColor(4);

    CherryGUI::EndChild();
    CherryGUI::PopStyleColor();

    CherryGUI::EndGroup();

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
inline Component &HeaderImageTextButton(
    const Identifier &identifier, const std::string &label,
    const std::string &image_path,
    const std::function<void()> &render_content = []() {}) {
  return CherryApp.PushComponent<Cherry::Components::HeaderImageTextButton>(
      identifier, label, image_path, render_content);
}

inline Component &HeaderImageTextButton(
    const std::string &label, const std::string &image_path,
    const std::function<void()> &render_content = []() {}) {
  return Cherry::Kit::HeaderImageTextButton(
      Application::GenerateUniqueID(label, image_path, render_content,
                                    "HeaderImageTextButton"),
      label, image_path, render_content);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_HEADER_IMAGE_TEXT_BUTTON
