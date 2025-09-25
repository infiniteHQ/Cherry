#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// BlockHorizontalCustom
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_BLOCK_HORIZONTAL_CUSTOM
#define CHERRY_KIT_BLOCK_HORIZONTAL_CUSTOM

namespace Cherry {
namespace Components {
class BlockHorizontalCustom : public Component {
public:
  BlockHorizontalCustom(
      const Cherry::Identifier &id,
      const std::vector<std::function<void()>> &renderCallbacks,
      const float &width = 260.0f, const float &height = 70.0f,
      const std::function<void()> &onClickedCallback = []() {})
      : Component(id), m_RenderCallbacks(renderCallbacks),
        m_OnClickedCallback(onClickedCallback) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("block_color", "theme:block_color");
    SetProperty("block_color_hovered", "theme:block_color_hovered");
    SetProperty("block_color_pressed", "theme:block_color_pressed");
    SetProperty("block_border_color", "theme:block_border_color");
    SetProperty("block_border_color_hovered",
                "theme:block_border_color_hovered");
    SetProperty("block_border_color_pressed",
                "theme:block_border_color_pressed");
    SetProperty("block_border_radius", "theme:block_border_radius");
    SetProperty("block_border_size", "theme:block_border_size");
    SetProperty("size_x", std::to_string(width));
    SetProperty("size_y", std::to_string(height));

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
    cachedSize = ImVec2(std::stof(GetProperty("size_x")),
                        std::stof(GetProperty("size_y")));

    background_color = HexToImU32(GetProperty("block_color"));
    background_color_hovered = HexToImU32(GetProperty("block_color_hovered"));
    background_color_pressed = HexToImU32(GetProperty("block_color_pressed"));

    border_color = HexToImU32(GetProperty("block_border_color"));
    border_color_hovered =
        HexToImU32(GetProperty("block_border_color_hovered"));
    border_color_pressed =
        HexToImU32(GetProperty("block_border_color_pressed"));

    border_size = std::stof(GetProperty("block_border_size"));
    cached_radius = std::stof(GetProperty("block_border_radius"));
    disabletime = GetProperty("disable_time") == "false";

    ImVec2 pos = CherryGUI::GetCursorScreenPos();

    CherryGUI::PushStyleVar(ImGuiStyleVar_ChildRounding, cached_radius);
    CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color);

    int style_props_opt = 0;

    bool isHovered = GetData("isHovered") == "true";
    bool isClicked = GetData("isClicked") == "true";
    bool isActivated = GetData("isActivated") == "true";
    bool isPressed = GetData("isPressed") == "true";

    if (isHovered) {
      if (disabletime)
        SetData("lastHovered", GetCurrentTime());
      CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_hovered);
      border_color = border_color_hovered;
      style_props_opt++;
    }

    if (isClicked) {
      if (disabletime)
        SetData("lastClicked", GetCurrentTime());
      CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_pressed);
      border_color = border_color_pressed;
      style_props_opt++;
    }

    if (isActivated) {
      if (disabletime)
        SetData("lastActivated", GetCurrentTime());
      CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_pressed);
      border_color = border_color_pressed;
    }

    if (isPressed) {
      if (disabletime)
        SetData("lastPressed", GetCurrentTime());
      CherryGUI::PushStyleColor(ImGuiCol_ChildBg, background_color_pressed);
      border_color = border_color_pressed;
      style_props_opt++;
    }

    SetData("isHovered", "false");
    SetData("isClicked", "false");
    SetData("isPressed", "false");
    SetData("isActivated", "false");

    std::string label = GetIdentifier().string() + "RenderCallbacks";

    if (CherryGUI::BeginChild(label.c_str(), cachedSize, false,
                          ImGuiWindowFlags_NoScrollbar |
                              ImGuiWindowFlags_NoScrollWithMouse)) {
      // Plus d'InvisibleButton qui bloque tout

      // Interaction souris sur la zone du Child
      if (CherryGUI::IsWindowHovered(
              ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) &&
          CherryGUI::IsMouseClicked(ImGuiMouseButton_Left)) {
        SetData("isClicked", "true");
        if (m_OnClickedCallback) {
          m_OnClickedCallback();
        }
      }

      if (CherryGUI::IsWindowHovered(
              ImGuiHoveredFlags_AllowWhenBlockedByPopup |
              ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
        CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
        SetData("isHovered", "true");
      }

      if (CherryGUI::IsItemActivated())
        SetData("isActivated", "true");

      if (CherryGUI::IsItemActive())
        SetData("isPressed", "true");

      // ✅ Aligner horizontalement les callbacks visibles
      for (size_t i = 0; i < m_RenderCallbacks.size(); ++i) {
        if (i > 0)
          CherryGUI::SameLine(0, 10.0f); // espacement horizontal
        m_RenderCallbacks[i]();
      }
    }
    CherryGUI::EndChild();

    CherryGUI::PopStyleVar();   // ChildRounding
    CherryGUI::PopStyleColor(); // ChildBg
    CherryGUI::PopStyleColor(style_props_opt);

    // ✅ Dessin du contour autour du bloc
    ImDrawList *drawList = CherryGUI::GetWindowDrawList();
    drawList->AddRect(pos, ImVec2(pos.x + cachedSize.x, pos.y + cachedSize.y),
                      border_color, cached_radius, 0, border_size);
  }

protected:
  ImVec2 cachedSize;
  ImU32 background_color;
  ImU32 background_color_hovered;
  ImU32 background_color_pressed;
  ImU32 border_color;
  ImU32 border_color_hovered;
  ImU32 border_color_pressed;
  float border_size;
  float cached_radius;
  bool disabletime;

private:
  std::function<void()> m_OnClickedCallback;
  std::vector<std::function<void()>> m_RenderCallbacks;
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
inline Component &BlockHorizontalCustom(
    const Identifier &identifier,
    const std::vector<std::function<void()>> &renderCallbacks) {
  return CherryApp.PushComponent<Cherry::Components::BlockHorizontalCustom>(
      identifier, renderCallbacks);
}

inline Component &BlockHorizontalCustom(
    const std::vector<std::function<void()>> &renderCallbacks) {
  return Cherry::Kit::BlockHorizontalCustom(
      Application::GenerateUniqueID(renderCallbacks, "BlockHorizontalCustom"),
      renderCallbacks);
}

inline Component &BlockHorizontalCustom(
    const Identifier &identifier,
    const std::function<void()> &onClickedCallback = []() {},
    const float &width = 260.0f, const float &height = 110.0f,
    const std::vector<std::function<void()>> &renderCallbacks = {}, int i = 0) {
  return CherryApp.PushComponent<Cherry::Components::BlockHorizontalCustom>(
      identifier, renderCallbacks, width, height, onClickedCallback);
}

inline Component &BlockHorizontalCustom(
    const std::function<void()> &onClickedCallback = []() {},
    const float &width = 260.0f, const float &height = 110.0f,
    const std::vector<std::function<void()>> &renderCallbacks = {}, int i = 0) {
  return Cherry::Kit::BlockHorizontalCustom(
      Application::GenerateUniqueID(onClickedCallback, width, height,
                                    renderCallbacks, i,
                                    "BlockHorizontalCustom"),
      onClickedCallback, width, height, renderCallbacks, i);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_BLOCK_HORIZONTAL_CUSTOM
