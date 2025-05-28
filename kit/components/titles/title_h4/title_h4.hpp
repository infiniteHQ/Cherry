#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TitleFour
// Summary : A big title.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TITLE_H4
#define CHERRY_KIT_TITLE_H4

namespace Cherry {
  namespace Components {
    class TitleFour : public Component {
     public:
      TitleFour(const Cherry::Identifier &id, const std::string &label) : Component(id) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_text", "#FFFFFFFF");  // TODO get the default theme

        // Informations
        SetProperty("label", label);
      }

      void Render() override {
        float oldsize = CherryGUI::GetFont()->Scale;
        CherryGUI::GetFont()->Scale *= 1.50;  // TODO : property
        CherryGUI::PushFont(CherryGUI::GetFont());

        CherryGUI::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 6));

        CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")), GetProperty("label").c_str());

        CherryGUI::PopStyleVar();

        CherryGUI::GetFont()->Scale = oldsize;
        CherryGUI::PopFont();
      }
    };
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline std::shared_ptr<Component> TitleFour(const std::string &label) {
      auto anonymous_id = Application::GenerateUniqueID(label);
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing;
      }

      auto button = Application::CreateAnonymousComponent<Components::TitleFour>(Components::TitleFour(anonymous_id, label));
      button->Render();
      return button;
    }

    inline std::shared_ptr<Component> TitleFour(const Cherry::Identifier &identifier, const std::string &label) {
      if (identifier.string() == "__inline") {
        auto new_title =
            Application::CreateAnonymousComponent<Components::TitleFour>(Components::TitleFour(identifier, label));
        new_title->Render();
        return new_title;
      }

      // Get the object if exist
      auto existing_title = Application::GetComponent(identifier);
      if (existing_title) {
        existing_title->Render();
      } else {
        // Create the object if not exist
        auto new_title = Application::CreateComponent<Components::TitleFour>(Components::TitleFour(identifier, label));
        new_title->Render();
        return new_title;
      }
      return existing_title;
    }
  }  // namespace Kit

}  // namespace Cherry

#endif  // CHERRY_KIT_TITLE_H4
