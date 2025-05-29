#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// KeyValCustom
//

#ifndef CHERRY_KIT_KEYVAL_CUSTOM
#define CHERRY_KIT_KEYVAL_CUSTOM

namespace Cherry {
  namespace Components {
    class KeyValCustom : public Component {
     public:
      KeyValCustom(const Cherry::Identifier &id, const std::string &label, const std::function<void()> &renderCallback)
          : Component(id),
            m_RenderCallback(renderCallback) {
        // Identifier
        SetIdentifier(id);

        // Colors
        SetProperty("color_text", "#454545B2");

        // Informations
        SetProperty("label", label);
      }

      void Render() override {
        auto parent = Cherry::GetParent();
        if (parent) {
          if (std::stoi(parent->GetData("renderedColumn")) == 0) {
            CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));
            CherryGUI::AlignTextToFramePadding();
            CherryGUI::Indent(10.0f);
            CherryGUI::TextWrapped(GetProperty("label").c_str());
            CherryGUI::Unindent(10.0f);
          } else if (std::stoi(parent->GetData("renderedColumn")) == 1) {
            std::string identifier = GetIdentifier().string();
            std::string Label = "####" + GetProperty("label");

            if (!identifier.empty()) {
              Label += Label + identifier;
            }

            CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg, IM_COL32(100, 100, 100, 40));

            if (m_RenderCallback) {
              m_RenderCallback();
            } else {
              CherryGUI::Text("INVALID VALUE");
            }
          }
        }
      }

     private:
      std::function<void()> m_RenderCallback;
    };
  }  // namespace Components

  // End-User API
  namespace Kit {
    inline std::shared_ptr<Component> KeyValCustom(const std::string &label, const std::function<void()> &renderCallback) {
      // Inline component
      auto anonymous_id = Application::GenerateUniqueID(label, renderCallback);
      auto existing = Application::GetAnonymousComponent(anonymous_id);
      if (existing) {
        existing->Render();
        return existing;
      } else {
        auto button = Application::CreateAnonymousComponent<Components::KeyValCustom>(
            Components::KeyValCustom(anonymous_id, label, renderCallback));
        button->Render();
        return button;
      }
    }

    inline std::shared_ptr<Component> KeyValCustom(
        const Cherry::Identifier &identifier,
        const std::string &label,
        const std::function<void()> &renderCallback) {
      // Get the object if exist
      auto existing_keyval = Application::GetComponent(identifier);
      if (existing_keyval) {
        existing_keyval->RefreshContextProperties();
        existing_keyval->Render();
      } else {
        // Create the object if not exist
        auto new_keyval = Application::CreateComponent<Components::KeyValCustom>(
            Components::KeyValCustom(identifier, label, renderCallback));
        new_keyval->RefreshContextProperties();
        new_keyval->Render();
        return new_keyval;
      }
      return existing_keyval;
    }
  }  // namespace Kit

}  // namespace Cherry

#endif  // CHERRY_KIT_KEYVAL_CUSTOM
