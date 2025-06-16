#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ProgressBarPercentage
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PROGRESS_BAR_PERCENTAGE
#define CHERRY_KIT_PROGRESS_BAR_PERCENTAGE

namespace Cherry {
namespace Components {
class ProgressBarPercentage : public Component {
public:
  ProgressBarPercentage(const Cherry::Identifier &id, float *percentage)
      : Component(id), m_Percentage(percentage) {
    // Identifier
    SetIdentifier(id);
  }

  void Render() override {
    float fraction = (*m_Percentage) / 100.0f;
    CherryGUI::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
  }

private:
  float *m_Percentage;
};

} // namespace Components

// End-User API
namespace Kit {
inline Component &ProgressBarPercentage(const Identifier &identifier,
                                        float *percentage) {
  return CherryApp.PushComponent<Cherry::Components::ProgressBarPercentage>(
      identifier, percentage);
}

inline Component &ProgressBarPercentage(float *percentage) {
  return Cherry::Kit::ProgressBarPercentage(
      Application::GenerateUniqueID(percentage, "ProgressBarPercentage"),
      percentage);
}
} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_PROGRESS_BAR_PERCENTAGE
