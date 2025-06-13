#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TextSimple
// Summary : A big text.
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TEXT_SIMPLE
#define CHERRY_KIT_TEXT_SIMPLE

namespace Cherry {
namespace Components {
class TextSimple : public Component {
public:
  TextSimple(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "#FFFFFFFF");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                           GetProperty("label").c_str());

    auto parent = Cherry::GetParent();
    if (parent) {
      CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                             parent->GetProperty("label").c_str());
    }
  }
};
} // namespace Components

// End-User API
namespace Kit {
inline std::shared_ptr<Component> TextSimple(const std::string &label) {
  // Inline component
  auto anonymous_id = Application::GenerateUniqueID(label);
  auto existing = Application::GetAnonymousComponent(anonymous_id);
  if (existing) {
    existing->RenderWrapper();
    return existing;
  } else {
    std::cout << "Create" << std::endl;
    auto button = Application::CreateAnonymousComponent<Components::TextSimple>(
        Components::TextSimple(anonymous_id, label));
    button->RenderWrapper();
    return button;
  }
}

inline std::shared_ptr<Component>
TextSimple(const Cherry::Identifier &identifier, const std::string &label) {
  // Get the object if exist
  auto existing_text = Application::GetComponent(identifier);
  if (existing_text) {
    existing_text->RenderWrapper();
  } else {
    // Create the object if not exist
    auto new_text = Application::CreateComponent<Components::TextSimple>(
        Components::TextSimple(identifier, label));
    new_text->RenderWrapper();
    return new_text;
  }
  return existing_text;
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TEXT_SIMPLE
