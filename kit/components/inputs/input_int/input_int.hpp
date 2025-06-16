#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputInteger
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_INTEGER
#define CHERRY_KIT_INPUT_INTEGER

namespace Cherry {
namespace Components {
class InputInteger : public Component {
public:
  InputInteger(const Cherry::Identifier &id, const std::string &label,
               int *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("step", "1");
    SetProperty("step_fast", "5");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    if (m_Value) {
      CherryGUI::InputInt(Label.c_str(), m_Value,
                          std::stoi(GetProperty("step")),
                          std::stoi(GetProperty("step_fast")));
    } else {
      CherryGUI::Text("INVALID VALUE");
    }
  }

private:
  int *m_Value;
};
} // namespace Components

// End-User API
namespace Kit {

inline Component &InputInteger(const Identifier &identifier,
                               const std::string &label, int *value) {
  return CherryApp.PushComponent<Cherry::Components::InputInteger>(
      identifier, label, value);
}

inline Component &InputInteger(const std::string &label, int *value) {
  return Cherry::Kit::InputInteger(
      Application::GenerateUniqueID(label, value, "InputInt"), label, value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_INPUT_INTEGER
