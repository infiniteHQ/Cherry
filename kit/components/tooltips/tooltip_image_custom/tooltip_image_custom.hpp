#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TooltipImageCustom
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TOOLTIP_IMAGE_CUSTOM
#define CHERRY_KIT_TOOLTIP_IMAGE_CUSTOM

namespace Cherry {
  namespace Components {
    class TooltipImageCustom : public Component {
     public:
      TooltipImageCustom(
          const Cherry::Identifier &id,
          const std::string &image_path,
          const std::function<void()> &render_callback)
          : Component(id),
            m_RenderCallback(render_callback) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_border", "#454545B2");
        SetProperty("color_border_hovered", "#454545B2");
        SetProperty("color_border_clicked", "#454545B2");
        SetProperty("color_bg", "#242424FF");
        SetProperty("color_bg_hovered", "#343434FF");
        SetProperty("color_bg_clicked", "#444444FF");

        // Informations
        SetProperty("image_path", image_path);
      }

      void Render() override {
        ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get_texture(GetProperty("image_path"));
        CherryGUI::Image(texture, ImVec2(15, 15));  // TODO properties

        if (CherryGUI::IsItemHovered()) {
          CherryGUI::BeginTooltip();
          CherryGUI::PushTextWrapPos(CherryGUI::GetFontSize() * 35.0f);
          if (m_RenderCallback) {
            m_RenderCallback();
          }
          CherryGUI::PopTextWrapPos();
          CherryGUI::EndTooltip();
        }
      }

     private:
      std::function<void()> m_RenderCallback;
    };
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline bool TooltipImageCustom(const std::string &image_path, const std::function<void()> &render_callback) {
      // Inline component
      auto anonymous_id = Application::GenerateUniqueID(image_path, render_callback);
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing->GetData("isClicked") == "true" ? true : false;
      } else {
        auto button = Application::CreateAnonymousComponent<Components::TooltipImageCustom>(
            Components::TooltipImageCustom(anonymous_id, image_path, render_callback));
        button->Render();
        return button->GetData("isClicked") == "true" ? true : false;
      }
      return false;
    }

    inline bool TooltipImageCustom(
        const Cherry::Identifier &identifier,
        const std::string &image_path,
        const std::function<void()> &render_callback) {
      // Get the object if exist
      auto existing_button = Application::GetComponent(identifier);
      if (existing_button) {
        existing_button->Render();
        return existing_button->GetData("isClicked") == "true" ? true : false;
      } else {
        // Create the object if not exist
        auto new_button = Application::CreateComponent<Components::TooltipImageCustom>(
            Components::TooltipImageCustom(identifier, image_path, render_callback));
        new_button->Render();
        return new_button->GetData("isClicked") == "true" ? true : false;
      }
    }
  }  // namespace Kit
}  // namespace Cherry

#endif  // CHERRY_KIT_TOOLTIP_IMAGE_CUSTOM
