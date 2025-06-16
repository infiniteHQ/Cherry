#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputDouble
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_DOUBLE
#define CHERRY_KIT_INPUT_DOUBLE

namespace Cherry {
namespace Components {
class InputDouble : public Component {
public:
  InputDouble(const Cherry::Identifier &id, const std::string &label,
              double *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    SetProperty("step", "1.0");
    SetProperty("step_fast", "5.0");

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
      CherryGUI::InputDouble(Label.c_str(), m_Value,
                             std::stof(GetProperty("step")),
                             std::stof(GetProperty("step_fast")));
    } else {
      CherryGUI::Text("INVALID VALUE");
    }
  }

private:
  double *m_Value;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &InputDouble(const Identifier &identifier,
                              const std::string &label, double *value) {
  return CherryApp.PushComponent<Cherry::Components::InputDouble>(identifier,
                                                                  label, value);
}

inline Component &InputDouble(const std::string &label, double *value) {
  return Cherry::Kit::InputDouble(
      Application::GenerateUniqueID(label, value, "InputDouble"), label, value);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_INPUT_DOUBLE
