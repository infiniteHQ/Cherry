#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// InputString
//

#ifndef CHERRY_KIT_INPUT_STRING
#define CHERRY_KIT_INPUT_STRING

namespace Cherry {
namespace Components {
class InputString : public Component {
public:
  InputString(const Cherry::Identifier &id, const std::string &label,
              std::string *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#454545B2");
    SetProperty("size_x", "30");
    SetProperty("padding_x", "7");
    SetProperty("padding_y", "7");
    SetProperty("enter_return", "false");
    SetProperty("focus_on_appear", "false");

    // Informations
    SetProperty("label", label);
    SetProperty("description", "");
    SetProperty("description_logo", "");
    SetProperty("description_logo_place", "r");

    // Data
    SetData("active", "false");
  }

  void Render() override {
    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!GetProperty("description_logo").empty()) {
      m_Logo = Application::Get().GetCurrentRenderedWindow()->get_texture(
          GetProperty("description_logo"));
    }

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    if (m_Value) {
      char buffer[256];
      std::strncpy(buffer, m_Value->c_str(), sizeof(buffer));

      float sizeX = std::stof(GetProperty("size_x"));
      float paddingX = std::stof(GetProperty("padding_x"));
      float paddingY = std::stof(GetProperty("padding_y"));
      std::string description = GetProperty("description");
      std::string logoPath = GetProperty("description_logo");
      std::string logoPlacement = GetProperty("description_logo_place");

      bool hasLogo = !logoPath.empty();
      ImTextureID logo = hasLogo ? m_Logo : nullptr;
      hasLogo = hasLogo && (logo != nullptr);

      float logoSize = 16.0f;
      float logoSpacing = 4.0f;

      CherryGUI::SetNextItemWidth(sizeX);
      CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding,
                              ImVec2(paddingX, paddingY));
      CherryGUI::PushID(Label.c_str());

      ImVec2 cursorPos = CherryGUI::GetCursorScreenPos();
      ImVec2 padding = CherryGUI::GetStyle().FramePadding;

      ImGuiInputTextFlags flags;

      if (GetProperty("enter_return") == "true") {
        flags | ImGuiInputTextFlags_EnterReturnsTrue;
      }

      bool active =
          CherryGUI::InputText(Label.c_str(), buffer, sizeof(buffer), flags);
      if (GetProperty("focus_on_appear") == "true") {
        CherryGUI::SetKeyboardFocusHere();
      }

      if (active) {
        SetData("active", "true");

      } else {
        SetData("active", "false");
      }

      float textX = cursorPos.x + padding.x + 4.0f;
      float textRight = cursorPos.x + sizeX - padding.x;

      if (hasLogo && logoPlacement == "l") {
        textX += logoSize + logoSpacing;
      }
      if (hasLogo && logoPlacement == "r") {
        textRight -= logoSize + logoSpacing;
      }

      float availableTextWidth = (std::max)(0.0f, textRight - textX);

      std::string displayedHint;
      const std::string ellipsis = "...";
      for (int len = (int)description.size(); len >= 0; --len) {
        std::string test = (len < (int)description.size())
                               ? description.substr(0, len) + ellipsis
                               : description;
        if (CherryGUI::CalcTextSize(test.c_str()).x <= availableTextWidth) {
          displayedHint = test;
          break;
        }
      }

      if (std::strlen(buffer) == 0) {
        float hintY = cursorPos.y + padding.y +
                      (CherryGUI::GetFontSize() -
                       CherryGUI::CalcTextSize(displayedHint.c_str()).y) *
                          0.5f;

        ImVec2 clipMin = ImVec2(textX, cursorPos.y);
        ImVec2 clipMax =
            ImVec2(textX + availableTextWidth,
                   cursorPos.y + CherryGUI::GetFontSize() + padding.y * 2.0f);

        CherryGUI::PushClipRect(clipMin, clipMax, true);
        CherryGUI::GetWindowDrawList()->AddText(
            ImVec2(textX, hintY), CherryGUI::GetColorU32(ImGuiCol_TextDisabled),
            displayedHint.c_str());
        CherryGUI::PopClipRect();
      }

      if (hasLogo) {
        float logoY = cursorPos.y + padding.y +
                      (CherryGUI::GetFontSize() - logoSize) * 0.5f;
        float logoX = (logoPlacement == "l")
                          ? cursorPos.x + padding.x
                          : cursorPos.x + sizeX - padding.x - logoSize;

        CherryGUI::GetWindowDrawList()->AddImage(
            logo, ImVec2(logoX, logoY),
            ImVec2(logoX + logoSize, logoY + logoSize));
      }

      CherryGUI::PopID();
      CherryGUI::PopStyleVar();
      *m_Value = std::string(buffer);

    } else {
      CherryGUI::Text("INVALID VALUE");
    }
  }

private:
  std::string *m_Value;
  ImTextureID m_Logo;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &InputString(const Identifier &identifier,
                              const std::string &label, std::string *value) {
  return CherryApp.PushComponent<Cherry::Components::InputString>(identifier,
                                                                  label, value);
}

inline Component &InputString(const std::string &label, std::string *value) {
  return Cherry::Kit::InputString(
      Application::GenerateUniqueID(label, value, "InputString"), label, value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_INPUT_STRING
