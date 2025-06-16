#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// Space
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_Space
#define CHERRY_KIT_Space

namespace Cherry {
namespace Components {
class Space : public Component {
public:
  Space(const Cherry::Identifier &id, const float &spacing) : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("spacing", std::to_string(spacing));
  }

  void Render() override {
    CherryGUI::BeginDisabled();
    CherryGUI::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
    CherryGUI::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.0f));
    CherryGUI::Button("", ImVec2(0, std::stof(GetProperty("spacing"))));
    CherryGUI::PopStyleColor(2);
    CherryGUI::EndDisabled();
  }
};
} // namespace Components

// End-User API
namespace Kit {

inline Component &Space(const Identifier &identifier, const float &spacing) {
  return CherryApp.PushComponent<Cherry::Components::Space>(identifier,
                                                            spacing);
}

inline Component &Space(const float &spacing) {
  return Cherry::Kit::Space(Application::GenerateUniqueID(spacing, "Space"),
                            spacing);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_Space
