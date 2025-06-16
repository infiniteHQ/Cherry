#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// T ableCustom
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TABLE_CUSTOM
#define CHERRY_KIT_TABLE_CUSTOM

namespace Cherry {
namespace Components {
class TableCustom : public Component {
public:
  TableCustom(const Cherry::Identifier &id, const std::string &label,
              const std::vector<std::function<void(int)>> &rows = {},
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
    SetProperty("selectable", "true");

    // Columns
    SetProperty("columns_number", "2");
    SetProperty("columns_name_0", "First");
    SetProperty("columns_name_1", "Second");
    SetProperty("columns_width_0", "20.0");
    SetProperty("columns_width_1",
                "20.0"); // Only default values, you can use any number of
                         // columns you want, juste name columns_name_n /
                         // columns_width_n (n = number)

    // Label
    SetProperty("label", label);

    SetData("renderedColumn", "-1");
  }

  void Render() override {
    ImGuiTableFlags flags = ImGuiTableFlags_Borders + m_OtherFlags;

    if (GetProperty("selectable") == "true") {
      flags + ImGuiSelectableFlags_SelectOnClick;
    }

    CherryGUI::PushStyleColor(ImGuiCol_Border,
                              Cherry::HexToImU32(GetProperty("color_border")));
    CherryGUI::PushStyleColor(ImGuiCol_TableBorderStrong,
                              Cherry::HexToImU32(GetProperty("color_border")));

    // ImGuiStyle style = CherryGUI::GetStyle();
    // m_HeaderCellPaddingX = style.CellPadding.x;
    // m_HeaderCellPaddingY = style.CellPadding.y;
    // m_RowsCellPaddingX = style.CellPadding.x;
    // m_RowsCellPaddingY = style.CellPadding.y;zz

    // ADD COLOR borsder

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
        std::string column_name_prop = "columns_name_" + std::to_string(i);
        std::string column_width_prop =
            GetProperty("columns_width_" + std::to_string(i));
        float column_width = column_width_prop == "undefined"
                                 ? 0.0f
                                 : std::stof(column_width_prop);

        if (i == std::stoi(GetProperty("columns_number")) - 1) {
          CherryGUI::TableSetupColumn(GetProperty(column_name_prop).c_str(),
                                      ImGuiTableColumnFlags_NoResize,
                                      column_width);
        } else {
          CherryGUI::TableSetupColumn(GetProperty(column_name_prop).c_str(), 0,
                                      column_width);
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
        CherryGUI::TableNextRow();
        for (int column = 0; column < std::stoi(GetProperty("columns_number"));
             column++) {
          CherryGUI::TableSetColumnIndex(column);
          SetData("renderedColumn", std::to_string(column));

          if (row) {
            row(column);
          }
        }
      }
      CherryGUI::PopStyleVar();
      CherryGUI::EndTable();
    }

    CherryGUI::PopStyleColor(2);
  }

private:
  ImGuiTableFlags m_OtherFlags;
  std::vector<std::function<void(int)>> m_Rows;
};
} // namespace Components

// End-User API
namespace Kit {
inline Component &
TableCustom(const Identifier &identifier, const std::string &label,
            const std::vector<std::function<void(int)>> &rows = {},
            ImGuiTableFlags flags = 0) {
  return CherryApp.PushComponent<Cherry::Components::TableCustom>(
      identifier, label, rows, flags);
}

inline Component &
TableCustom(const std::string &label,
            const std::vector<std::function<void(int)>> &rows = {},
            ImGuiTableFlags flags = 0) {
  return Cherry::Kit::TableCustom(
      Application::GenerateUniqueID(label, rows, flags, "TableCustom"), label,
      rows, flags);
}

} // namespace Kit

} // namespace Cherry

#endif // CHERRY_KIT_TABLE_CUSTOM
