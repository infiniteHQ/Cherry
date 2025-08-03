#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// KeyValParent
//

#ifndef CHERRY_KIT_KEYVAL_PARENT
#define CHERRY_KIT_KEYVAL_PARENT

namespace Cherry {
namespace Components {

// TODO m_IsOpen also needs to be a cherry property

class KeyValParent : public Component {
public:
  KeyValParent(const Cherry::Identifier &id, const std::string &label,
               const std::vector<Component> &rows = {},
               bool defaultOpen = false)
      : Component(id), m_IsOpen(defaultOpen) {
    SetIdentifier(id);
    SetProperty("label", label);

    SetProperty("padding_x", "7");
    SetProperty("padding_y", "7");

    for (auto &row : rows) {
      m_Rows.push_back(CherryApp.GetComponentPtr(row.GetIdentifier()));
    }

    SetProperty("image_dropdown_up", Cherry::GetPath("resources/base/up.png"));
    SetProperty("image_dropdown_down",
                Cherry::GetPath("resources/base/down.png"));
  }

  void Render() override {
    auto parent = Cherry::GetParent();
    if (!parent)
      return;

    if (std::stoi(parent->GetData("renderedColumn")) == 0) {
      float row_height = CherryGUI::TableGetHeaderRowHeight();
      float row_width = CherryGUI::GetContentRegionAvail().x;

      ImVec2 selectable_pos = CherryGUI::GetCursorScreenPos();

      if (CherryGUI::TableGetColumnIndex() == 0) {
        CherryGUI::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
        CherryGUI::PushStyleColor(ImGuiCol_HeaderHovered,
                                  ImVec4(1, 1, 1, 0.1f));
        CherryGUI::PushStyleColor(ImGuiCol_HeaderActive,
                                  ImVec4(1, 1, 1, 0.15f));

        bool selected = CherryGUI::Selectable(
            ("##fullrow_hoverzone_" + GetIdentifier().string()).c_str(), false,
            ImGuiSelectableFlags_SpanAllColumns, ImVec2(0.0f, row_height));

        CherryGUI::PopStyleColor(3);

        if (selected) {
          m_IsOpen = !m_IsOpen;
        }
        CherryGUI::SetCursorScreenPos(selectable_pos);
      }

      CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                 IM_COL32(170, 170, 170, 40));
      CherryGUI::AlignTextToFramePadding();

      float padding = std::stof(parent->GetData("padding"));
      CherryGUI::Indent(10.0f);
      CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + padding);

      CherryGUI::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
      CherryGUI::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
      CherryGUI::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
      CherryGUI::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
      if (m_IsOpen) {
        CherryGUI::ArrowButton("##dropdown_arrow", ImGuiDir_Down);
      } else {
        CherryGUI::ArrowButton("##dropdown_arrow", ImGuiDir_Right);
      }
      CherryGUI::PopStyleColor(4);

      CherryGUI::SameLine();
      CherryGUI::TextWrapped(GetProperty("label").c_str());
      CherryGUI::Unindent(10.0f);
    } else if (std::stoi(parent->GetData("renderedColumn")) == 1) {
      CherryGUI::TableSetBgColor(ImGuiTableBgTarget_CellBg,
                                 IM_COL32(170, 170, 170, 40));
    }

    if (m_IsOpen && std::stoi(parent->GetData("renderedColumn")) == 1) {
      CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + 20.0f);
      float padding = std::stof(parent->GetData("padding"));

      if (CherryApp.m_ParentComponentsStack.size() >= 1) {
        float add_padding = 18.0f * CherryApp.m_ParentComponentsStack.size();
        padding += add_padding;
      }

      SetData("padding", std::to_string(padding));
      SetProperty("columns_number", parent->GetProperty("columns_number"));

      for (auto &child_comp : m_Rows) {
        if (!child_comp)
          continue;

        CherryGUI::TableNextRow();

        for (int column = 0;
             column < std::stoi(parent->GetProperty("columns_number"));
             column++) {
          CherryGUI::TableSetColumnIndex(column);
          SetData("renderedColumn", std::to_string(column));
          CherryApp.PushParentComponent(
              CherryApp.GetComponentPtr(this->GetIdentifier()));
          CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() + 20.0f);
          child_comp->Render();
          CherryApp.PopParentComponent();
        }
      }
      CherryGUI::SetCursorPosX(CherryGUI::GetCursorPosX() - 20.0f);
    }
  }

private:
  std::vector<std::shared_ptr<Component>> m_Rows;
  bool m_IsOpen;
};

} // namespace Components

// End-User API
namespace Kit {
inline Component &KeyValParent(const Identifier &identifier,
                               const std::string &label,
                               const std::vector<Component> &rows = {}) {
  return CherryApp.PushComponent<Cherry::Components::KeyValParent>(identifier,
                                                                   label, rows);
}

inline Component &KeyValParent(const std::string &label,
                               const std::vector<Component> &rows = {}) {
  return Cherry::Kit::KeyValParent(
      Application::GenerateUniqueID(label, rows, "KeyValParent"), label, rows);
}

inline Component &KeyValParent(const Identifier &identifier,
                               const std::string &label,
                               bool default_opened = false,
                               const std::vector<Component> &rows = {}) {
  return CherryApp.PushComponent<Cherry::Components::KeyValParent>(
      identifier, label, rows, default_opened);
}

inline Component &KeyValParent(const std::string &label,
                               bool default_opened = false,
                               const std::vector<Component> &rows = {}) {
  return Cherry::Kit::KeyValParent(
      Application::GenerateUniqueID(label, rows, "KeyValParent"), label,
      default_opened, rows);
}
} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_KEYVAL_PARENT
