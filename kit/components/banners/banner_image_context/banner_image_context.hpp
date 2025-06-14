#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// BannerImageContext
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BANNER_IMAGE_CONTEXT
#define CHERRY_KIT_BANNER_IMAGE_CONTEXT

namespace Cherry {
namespace Components {
class BannerImageContext : public Component {
public:
  BannerImageContext(
      const Cherry::Identifier &id, const std::string &image_path,
      const std::string &title, const std::string &description,
      const float &width = 260.0f, const float &height = 110.0f,
      const std::function<void()> &onClickedCallback = []() {})
      : Component(id), m_OnClickedCallback(onClickedCallback) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("block_color", "#252525");
    SetProperty("block_color_hovered", "#454545");
    SetProperty("block_color_pressed", "#555555");
    SetProperty("block_border_color", "#353535");
    SetProperty("block_border_color_hovered", "#353535");
    SetProperty("block_border_color_pressed", "#555555");
    SetProperty("block_border_radius", std::to_string(0.0f));
    SetProperty("block_border_size", std::to_string(1.0f));
    SetProperty("size_x", std::to_string(width));
    SetProperty("size_y", std::to_string(height));
    SetProperty("image_path", image_path);
    SetProperty("title", title);
    SetProperty("description", description);

    // Data & User-level informations
    SetData("isClicked", "false");
    SetData("isPressed", "false");
    SetData("isHovered", "false");
    SetData("isActivated", "false");
    SetData("lastClicked", "never");
    SetData("lastPressed", "never");
    SetData("lastHovered", "never");
    SetData("lastActivated", "never");
  }

  void Render() override {
    ImVec2 squareSize(std::stof(GetProperty("size_x")),
                      std::stof(GetProperty("size_y")));

    std::string title = GetProperty("title");
    const char *originalText = title.c_str();
    std::string description = GetProperty("description");
    char truncatedText[32];

    std::string image = GetProperty("image_path");

    if (strlen(originalText) > 24) {
      strncpy(truncatedText, originalText, 8);
      strcpy(truncatedText + 8, "...");
    } else {
      strcpy(truncatedText, originalText);
    }

    ImVec2 textSize = CherryGUI::CalcTextSize(truncatedText);
    ImVec2 totalSize(squareSize.x, squareSize.y + textSize.y + 5);

    ImVec2 cursorPos = CherryGUI::GetCursorScreenPos();

    if (CherryGUI::InvisibleButton(originalText, totalSize)) {
      /*if (link_path != "")
      {
          OpenURL(link_path);
      }*/
    }

    if (CherryGUI::IsItemHovered()) {
      CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
    }

    ImDrawList *drawList = CherryGUI::GetWindowDrawList();

    if (!image.empty() && std::filesystem::exists(image)) {
      drawList->AddImage(
          Cherry::GetTexture(image), cursorPos,
          ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y));
    } else {
      drawList->AddImage(
          Cherry::GetTexture(image), cursorPos,
          ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y));
    }

    {
      ImVec2 textPadding(10.0f, 5.0f);
      float blurHeight = 30.0f;

      ImVec2 blurPos =
          ImVec2(cursorPos.x, cursorPos.y + squareSize.y - blurHeight);
      ImVec2 blurSize =
          ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y);

      drawList->AddRectFilled(blurPos, blurSize, IM_COL32(0, 0, 0, 128));

      ImVec2 textPos =
          ImVec2(cursorPos.x + textPadding.x,
                 cursorPos.y + squareSize.y - blurHeight + textPadding.y);

      drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), title.c_str());
    }

    ImVec2 textPos = ImVec2(cursorPos.x, cursorPos.y + squareSize.y + 5);
    float maxWidth = squareSize.x;

    std::string line;
    float lineWidth = 0.0f;
    for (char ch : description) {
      ImVec2 charSize = CherryGUI::CalcTextSize(std::string(1, ch).c_str());
      if (lineWidth + charSize.x > maxWidth && !line.empty()) {
        drawList->AddText(textPos, Cherry::HexToImU32("#999999ff"),
                          line.c_str());
        textPos.y += charSize.y;
        line.clear();
        lineWidth = 0.0f;
      }
      line += ch;
      lineWidth += charSize.x;
    }

    if (!line.empty()) {
      drawList->AddText(textPos, Cherry::HexToImU32("#999999ff"), line.c_str());
    }

    ImU32 textColor = IM_COL32(255, 255, 255, 255);
    ImU32 highlightColor = IM_COL32(255, 255, 0, 255);
    ImU32 highlightTextColor = IM_COL32(0, 0, 0, 255);

    if (CherryGUI::IsItemHovered()) {
      CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
      drawList->AddRect(
          cursorPos,
          ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y),
          IM_COL32(135, 135, 135, 255), 0.0f, 0, 2.0f);
    }
  }

private:
  std::function<void()> m_OnClickedCallback;
  std::string GetCurrentTime() {
    std::string m_LastClickTime;
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    m_LastClickTime = std::ctime(&now_c);
    m_LastClickTime.erase(m_LastClickTime.length() - 1);
    return m_LastClickTime;
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &BannerImageContext(const Cherry::Identifier &identifier,
                                     const std::string &image_path = "",
                                     const std::string &title = "",
                                     const std::string &description = "") {
  return CherryApp.PushComponent<Cherry::Components::BannerImageContext>(
      identifier, image_path, title, description);
}

inline Component &BannerImageContext(
    const Cherry::Identifier &identifier, const float &width = 260.0f,
    const float &height = 110.0f, const std::string &image_path = "",
    const std::string &title = "", const std::string &description = "",
    const std::function<void()> &onClickedCallback = []() {}) {
  return CherryApp.PushComponent<Cherry::Components::BannerImageContext>(
      identifier, image_path, title, description, width, height,
      onClickedCallback);
}

inline Component &BannerImageContext(const std::string &image_path = "",
                                     const std::string &title = "",
                                     const std::string &description = "") {
  return Cherry::Kit::BannerImageContext(
      Application::GenerateUniqueID(image_path, title, description), image_path,
      title, description);
}

inline Component &BannerImageContext(
    const float &width = 260.0f, const float &height = 110.0f,
    const std::string &image_path = "", const std::string &title = "",
    const std::string &description = "",
    const std::function<void()> &onClickedCallback = []() {}) {
  return Cherry::Kit::BannerImageContext(
      Application::GenerateUniqueID(width, height, image_path, title,
                                    description, onClickedCallback),
      width, height, image_path, title, description, onClickedCallback);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BANNER_IMAGE_CONTEXT
