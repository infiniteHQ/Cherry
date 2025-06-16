#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// PlotHistorigramFloat
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_PLOT_HISTORIGRAM_FLOAT
#define CHERRY_KIT_PLOT_HISTORIGRAM_FLOAT

namespace Cherry {
namespace Components {
class PlotHistorigramFloat : public Component {
public:
  PlotHistorigramFloat(const Cherry::Identifier &id, const std::string &label,
                       const std::vector<float> &float_table, int number_showed)
      : Component(id), m_FloatTable(float_table) {
    // Identifier
    SetIdentifier(id);

    // Informations
    SetProperty("label", label);
    SetProperty("number_showed", std::to_string(number_showed));
  }

  // Streamed int mode
  PlotHistorigramFloat(const Cherry::Identifier &id, const std::string &label,
                       const std::vector<float> &float_table,
                       int *number_showed)
      : Component(id), m_FloatTable(float_table),
        m_NumberShowed(number_showed) {
    // Identifier
    SetIdentifier(id);

    // Informations
    SetProperty("label", label);
    SetProperty("number_showed", std::to_string(*number_showed));
  }

  void Render() override {
    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    int number_showed;
    if (m_NumberShowed) {
      number_showed = *m_NumberShowed;
    } else {
      number_showed = std::stoi(GetProperty("number_showed"));
    }

    int totalSize = static_cast<int>(m_FloatTable.size());
    int size = (number_showed == -1 || number_showed > totalSize)
                   ? totalSize
                   : number_showed;

    if (!m_FloatTable.empty() && size > 0) {
      const float *dataStart = m_FloatTable.data() + (totalSize - size);
      CherryGUI::PlotHistogram(Label.c_str(), dataStart, size);
    }
  }

private:
  std::vector<float> m_FloatTable;
  int *m_NumberShowed = nullptr;
};

} // namespace Components

// End-User API
namespace Kit {
inline Component &PlotHistorigramFloat(const Identifier &identifier,
                                       const std::string &label,
                                       const std::vector<float> &float_table,
                                       int number_showed = -1) {
  return CherryApp.PushComponent<Cherry::Components::PlotHistorigramFloat>(
      identifier, label, float_table, number_showed);
}

inline Component &PlotHistorigramFloat(const std::string &label,
                                       const std::vector<float> &float_table,
                                       int number_showed = -1) {
  return Cherry::Kit::PlotHistorigramFloat(
      Application::GenerateUniqueID(label, float_table, number_showed,
                                    "PlotHistorigramFloat"),
      label, float_table, number_showed);
}

inline Component &PlotHistorigramFloat(const Identifier &identifier,
                                       const std::string &label,
                                       const std::vector<float> &float_table,
                                       int *number_showed) {
  return CherryApp.PushComponent<Cherry::Components::PlotHistorigramFloat>(
      identifier, label, float_table, number_showed);
}

inline Component &PlotHistorigramFloat(const std::string &label,
                                       const std::vector<float> &float_table,
                                       int *number_showed) {
  return Cherry::Kit::PlotHistorigramFloat(
      Application::GenerateUniqueID(label, float_table, number_showed,
                                    "PlotHistorigramFloat"),
      label, float_table, number_showed);
}

} // namespace Kit
} // namespace Cherry

#endif // CHERRY_KIT_PLOT_HISTORIGRAM_FLOAT
