#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ProgressBarPercentageFloat
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_FLOAT
#define CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_FLOAT

namespace Cherry {
namespace Components {
class ProgressBarPercentageFloat : public Component {
public:
  ProgressBarPercentageFloat(const Cherry::Identifier &id, float *number,
                             float *goal)
      : Component(id), m_Number(number), m_Goal(goal) {
    // Identifier
    SetIdentifier(id);
  }

  void Render() override {
    float fraction = (*m_Number) / (*m_Goal);
    CherryGUI::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
  }

private:
  float *m_Number;
  float *m_Goal;
};

} // namespace Components

// End-User API
namespace Kit {
inline Component &ProgressBarPercentageFloat(const Identifier &identifier,
                                             float *number, float *goal) {
  return CherryApp
      .PushComponent<Cherry::Components::ProgressBarPercentageFloat>(
          identifier, number, goal);
}

inline Component &ProgressBarPercentageFloat(float *number, float *goal) {
  return Cherry::Kit::ProgressBarPercentageFloat(
      Application::GenerateUniqueID(number, goal, "ProgressBarPercentageFloat"),
      number, goal);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_FLOAT
