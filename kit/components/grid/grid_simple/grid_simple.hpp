#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// GridSimple
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_GRID_SIMPLE
#define CHERRY_KIT_GRID_SIMPLE

namespace Cherry {
namespace Components {
class GridSimple : public Component {
public:
  GridSimple(const Cherry::Identifier &id,
             std::vector<std::function<void()>> *elements,
             const float &blockSize = 250.0f,
             const float &minBlockSize = 250.0f)
      : Component(id), m_Elements(elements) {
    SetProperty("blockSize", std::to_string(blockSize));
    SetProperty("minBlockSize", std::to_string(minBlockSize));
  }

  GridSimple(const Cherry::Identifier &id,
             std::vector<std::shared_ptr<Cherry::Component>> *elements,
             const float &blockSize = 250.0f,
             const float &minBlockSize = 250.0f)
      : Component(id), m_Components(elements) {
    SetProperty("blockSize", std::to_string(blockSize));
    SetProperty("minBlockSize", std::to_string(minBlockSize));
  }

  void Render() override {
    float block_size = std::stof(GetProperty("blockSize"));
    float min_block_size = std::stof(GetProperty("minBlockSize"));

    ImVec2 avail = CherryGUI::GetContentRegionAvail();
    int columns = (std::max)(1, static_cast<int>(avail.x / block_size));
    float actual_block_size = (std::max)(min_block_size, avail.x / columns);

    int index = 0;

    if (m_Elements != nullptr) {
      if (!m_Elements->empty()) {
        for (const auto &element : *m_Elements) {
          if (element) {
            if (index > 0 && index % columns == 0)
              CherryGUI::NewLine();

            CherryGUI::BeginGroup();

            std::string id = GetIdentifier().string() + std::to_string(index);
            CherryGUI::PushID(id.c_str());
            CherryGUI::SetNextItemWidth(actual_block_size);

            element();

            CherryGUI::PopID();
            CherryGUI::EndGroup();

            if (index < m_Elements->size() - 1) {
              CherryGUI::SameLine();
            }
            index++;
          }
        }
      }
    }

    if (m_Components != nullptr) {
      if (!m_Components->empty()) {
        for (const auto &component : *m_Components) {
          if (component) {
            if (index > 0 && index % columns == 0)
              CherryGUI::NewLine();

            CherryGUI::BeginGroup();
            std::string id = GetIdentifier().string() + std::to_string(index);
            CherryGUI::PushID(id.c_str());
            CherryGUI::SetNextItemWidth(actual_block_size);

            component->Render();

            CherryGUI::PopID();
            CherryGUI::EndGroup();

            if (index < m_Components->size() - 1) {
              CherryGUI::SameLine();
            }
            index++;
          }
        }
      }
    }
  }

private:
  std::vector<std::function<void()>> *m_Elements = nullptr;
  std::vector<std::shared_ptr<Cherry::Component>> *m_Components = nullptr;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &GridSimple(
    const Identifier &identifier, const float &blockSize = 250.0f,
    const float &minBlockSize = 250.0f,
    std::vector<std::shared_ptr<Cherry::Component>> *elements = nullptr) {
  return CherryApp.PushComponent<Cherry::Components::GridSimple>(
      identifier, elements, blockSize, minBlockSize);
}

inline Component &GridSimple(
    const float &blockSize = 250.0f, const float &minBlockSize = 250.0f,
    std::vector<std::shared_ptr<Cherry::Component>> *elements = nullptr) {
  return Cherry::Kit::GridSimple(
      Application::GenerateUniqueID(blockSize, minBlockSize, elements,
                                    "GridSimple"),
      blockSize, minBlockSize, elements);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_GRID_SIMPLE
