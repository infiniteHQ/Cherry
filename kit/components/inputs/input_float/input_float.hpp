#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// InputFloat
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_INPUT_FLOAT
#define CHERRY_KIT_INPUT_FLOAT

namespace Cherry {
namespace Components {
class InputFloat : public Component {
public:
  InputFloat(const Cherry::Identifier &id, const std::string &label,
             float *value)
      : Component(id), m_Value(value) {
    // Identifier
    SetIdentifier(id);

    // Colors
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
      CherryGUI::InputFloat(Label.c_str(), m_Value,
                            std::stof(GetProperty("step")),
                            std::stof(GetProperty("step_fast")));
    } else {
      CherryGUI::Text("INVALID VALUE");
    }
  }

private:
  float *m_Value;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &InputFloat(const Identifier &identifier,
                             const std::string &label, float *value) {
  return CherryApp.PushComponent<Cherry::Components::InputFloat>(identifier,
                                                                 label, value);
}

inline Component &InputFloat(const std::string &label, float *value) {
  return Cherry::Kit::InputFloat(Application::GenerateUniqueID(label, value),
                                 label, value);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_INPUT_FLOAT
