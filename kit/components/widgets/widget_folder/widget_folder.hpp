#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// WidgetFolder
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_WIDGET_FOLDER
#define CHERRY_KIT_WIDGET_FOLDER

namespace Cherry {
namespace Components {
class WidgetFolder : public Component {
public:
  WidgetFolder(const Cherry::Identifier &id,
               const std::string &color = "#CC9900",
               const float &size_x = 150.0f, const float &size_y = 150.0f)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_folder", color);
    SetProperty("size_x", std::to_string(size_x));
    SetProperty("size_y", std::to_string(size_y));

    // Data & User-level informations
    SetData("isClicked", "false");
    SetData("isDoubleClicked", "false");
    SetData("isPressed", "false");
    SetData("isHovered", "false");
    SetData("isActivated", "false");
    SetData("lastClicked", "never");
    SetData("lastPressed", "never");
    SetData("lastHovered", "never");
    SetData("lastActivated", "never");
  }

  void Render() override {
    ImVec2 pos = CherryGUI::GetCursorScreenPos();

    ImVec2 size = ImVec2(std::stof(GetProperty("size_x")),
                         std::stof(GetProperty("size_y")));
    ImU32 color = HexToImU32(GetProperty("color_folder"));

    ImVec4 colVec = CherryGUI::ColorConvertU32ToFloat4(color);
    ImU32 flapColor = CherryGUI::ColorConvertFloat4ToU32(
        ImVec4(colVec.x * 0.8f, colVec.y * 0.8f, colVec.z * 0.8f, colVec.w));
    ImU32 shadowColor = CherryGUI::ColorConvertFloat4ToU32(ImVec4(
        colVec.x * 0.7f, colVec.y * 0.7f, colVec.z * 0.7f, colVec.w * 0.2f));

    auto isClicked =
        CherryGUI::InvisibleButton(GetIdentifier().string().c_str(), size);
    if (CherryGUI::IsItemHovered() &&
        CherryGUI::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
      SetData("isDoubleClicked", "true");
    }

    if (CherryGUI::IsItemHovered()) {
      SetData("isHovered", "true");
    }

    if (CherryGUI::IsItemActivated()) {
      SetData("isActivated", "true");
    }

    if (CherryGUI::IsItemActive()) {
      SetData("isPressed", "true");
    }

    if (isClicked) {
      SetData("isClicked", "true");
    }

    ImDrawList *drawList = CherryGUI::GetWindowDrawList();

    float flapHeight = size.y * 0.20f;
    float flapWidth = size.x * 0.4f;
    float borderRadius = size.y * 0.05f;

    ImVec2 flapTopLeft = ImVec2(pos.x + size.x * 0.001f, pos.y - 2.0f);
    ImVec2 flapBottomRight =
        ImVec2(flapTopLeft.x + flapWidth, pos.y + flapHeight);

    ImVec2 flapBarTopLeft = ImVec2(pos.x, pos.y + flapHeight - size.y * 0.14f);
    ImVec2 flapBarBottomRight =
        ImVec2(pos.x + size.x, pos.y + flapHeight + size.y * 0.07f);

    ImVec2 bodyTopLeft = ImVec2(pos.x, pos.y + flapHeight);
    ImVec2 bodyBottomRight = ImVec2(pos.x + size.x, pos.y + size.y);

    drawList->AddRectFilled(flapTopLeft, flapBottomRight, flapColor,
                            borderRadius, ImDrawFlags_RoundCornersTopLeft);
    drawList->AddRectFilled(flapBarTopLeft, flapBarBottomRight, flapColor,
                            borderRadius);

    drawList->AddRectFilled(bodyTopLeft, bodyBottomRight, color, borderRadius);

    float lineOffset = size.y * 0.05f;
    for (int i = 1; i <= 3; i++) {
      float yPos = bodyBottomRight.y - (lineOffset * i);
      drawList->AddLine(ImVec2(bodyTopLeft.x + size.x * 0.01f, yPos),
                        ImVec2(bodyBottomRight.x - size.x * 0.01f, yPos),
                        shadowColor, 2.0f);
    }

    if (CherryGUI::IsItemHovered()) {
      CherryGUI::SetMouseCursor(ImGuiMouseCursor_Hand);
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &WidgetFolder(const Identifier &identifier,
                               const std::string &color = "#CC9900",
                               const float &size_x = 150.0f,
                               const float &size_y = 150.0f) {
  return CherryApp.PushComponent<Cherry::Components::WidgetFolder>(
      identifier, color, size_x, size_y);
}

inline Component &WidgetFolder(const std::string &color = "#CC9900",
                               const float &size_x = 150.0f,
                               const float &size_y = 150.0f) {
  return Cherry::Kit::WidgetFolder(
      Application::GenerateUniqueID(color, size_x, size_y, "WidgetFolder"),
      color, size_x, size_y);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_WIDGET_FOLDER
