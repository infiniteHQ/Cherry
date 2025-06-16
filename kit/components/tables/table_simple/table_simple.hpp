#pragma once
#include "../../../../platform/engine/app/app.hpp"
#include "../../../../platform/engine/components/components.hpp"

//
// TableSimple
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TABLE_SIMPLE
#define CHERRY_KIT_TABLE_SIMPLE

namespace Cherry {
namespace Components {
class TableSimple : public Component {
public:
  TableSimple(const Cherry::Identifier &id, const std::string &label,
              const std::vector<std::shared_ptr<Component>> &rows = {},
              ImGuiTableFlags flags = 0)
      : Component(id), m_Rows(rows), m_OtherFlags(flags) {
    // Identifier
    SetIdentifier(id);

    // Padding
    SetProperty("cell_padding_x_header", "theme:table_cell_padding_x_header");
    SetProperty("cell_padding_y_header", "theme:table_cell_padding_y_header");
    SetProperty("cell_padding_x_row", "theme:table_cell_padding_x_row");
    SetProperty("cell_padding_y_row", "theme:table_cell_padding_y_row");

    // Style
    SetProperty("color_border", "theme:table_color_border");

    // Visibility
    SetProperty("header_visible", "true");
    SetProperty("header_text_visible", "true");

    // Columns
    SetProperty("columns_number", "2");
    SetProperty("columns_name_0", "First");
    SetProperty(
        "columns_name_1",
        "Second"); // Only default values, you can use any number of columns you
                   // want, just name columns_name_n (n = number)

    // Label
    SetProperty("label", label);

    SetData("renderedColumn", "-1");
  }

  void Render() override {
    ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | m_OtherFlags;

    // ImGuiStyle style = CherryGUI::GetStyle();
    // m_HeaderCellPaddingX = style.CellPadding.x;
    // m_HeaderCellPaddingY = style.CellPadding.y;
    // m_RowsCellPaddingX = style.CellPadding.x;
    // m_RowsCellPaddingY = style.CellPadding.y;

    std::string identifier = GetIdentifier().string();
    std::string Label = GetProperty("label");

    if (!identifier.empty()) {
      Label += "####" + identifier;
    }

    if (CherryGUI::BeginTable(
            Label.c_str(), std::stoi(GetProperty("columns_number")), flags)) {
      CherryGUI::PushStyleVar(
          ImGuiStyleVar_CellPadding,
          ImVec2(std::stof(GetProperty("cell_padding_x_header")),
                 std::stof(GetProperty("cell_padding_y_header"))));

      for (int i = 0; i < std::stoi(GetProperty("columns_number")); ++i) {
        if (i == std::stoi(GetProperty("columns_number")) - 1) {
          std::string column_name_prop = "columns_name_" + std::to_string(i);
          std::string final_name = GetProperty("header_text_visible") == "true"
                                       ? ""
                                       : "###" + GetProperty(column_name_prop);

          CherryGUI::TableSetupColumn(final_name.c_str(),
                                      ImGuiTableColumnFlags_NoResize);
        } else {
          std::string column_name_prop = "columns_name_" + std::to_string(i);
          std::string final_name = GetProperty("header_text_visible") == "true"
                                       ? ""
                                       : "###" + GetProperty(column_name_prop);
          CherryGUI::TableSetupColumn(final_name.c_str());
        }
      }

      if (GetProperty("header_visible") == "true") {
        CherryGUI::TableHeadersRow();
      }

      CherryGUI::PopStyleVar();

      CherryGUI::PushStyleVar(
          ImGuiStyleVar_CellPadding,
          ImVec2(std::stof(GetProperty("cell_padding_x_row")),
                 std::stof(GetProperty("cell_padding_y_row"))));
      for (auto row : m_Rows) {
        if (row) {
          CherryGUI::TableNextRow();
          for (int column = 0;
               column < std::stoi(GetProperty("columns_number")); column++) {
            CherryGUI::TableSetColumnIndex(column);
            SetData("renderedColumn", std::to_string(column));
            row->Render();
          }
        }
      }
      CherryGUI::PopStyleVar();
      CherryGUI::EndTable();
    }
  }

private:
  ImGuiTableFlags m_OtherFlags;
  std::vector<std::shared_ptr<Component>> m_Rows;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &
TableSimple(const Identifier &identifier, const std::string &label,
            const std::vector<std::shared_ptr<Component>> &rows = {},
            ImGuiTableFlags flags = 0) {
  return CherryApp.PushComponent<Cherry::Components::TableSimple>(
      identifier, label, rows, flags);
}

inline Component &
TableSimple(const std::string &label,
            const std::vector<std::shared_ptr<Component>> &rows = {},
            ImGuiTableFlags flags = 0) {
  return Cherry::Kit::TableSimple(
      Application::GenerateUniqueID(label, rows, flags, "TableSimple"), label,
      rows, flags);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TABLE_SIMPLE
