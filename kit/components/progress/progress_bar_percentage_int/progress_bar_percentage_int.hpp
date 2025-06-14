#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// ProgressBarPercentageInteger
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_INTEGER
#define CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_INTEGER

namespace Cherry {
namespace Components {
class ProgressBarPercentageInteger : public Component {
public:
  ProgressBarPercentageInteger(const Cherry::Identifier &id, int *number,
                               int *goal)
      : Component(id), m_Number(number), m_Goal(goal) {
    // Identifier
    SetIdentifier(id);
  }

  void Render() override {
    float fraction =
        static_cast<float>(*m_Number) / static_cast<float>(*m_Goal);
    CherryGUI::ProgressBar(fraction, ImVec2(-1.0f, 0.0f));
  }

private:
  int *m_Number;
  int *m_Goal;
};

} // namespace Components

// End-User API
namespace Kit {
inline Component &ProgressBarPercentageInteger(const Identifier &identifier,
                                               int *number, int *goal) {
  return CherryApp
      .PushComponent<Cherry::Components::ProgressBarPercentageInteger>(
          identifier, number, goal);
}

inline Component &ProgressBarPercentageInteger(int *number, int *goal) {
  return Cherry::Kit::ProgressBarPercentageInteger(
      Application::GenerateUniqueID(number, goal), number, goal);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_PROGRESS_BAR_PERCENTAGE_INTEGER
