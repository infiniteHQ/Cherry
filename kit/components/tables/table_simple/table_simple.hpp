#pragma once
#include "../../../../platform/engine/app.hpp"
#include "../../../../platform/engine/components.hpp"

//
// TableSimple
// Authors : Infinite, Diego Moreno
//

#ifndef CHERRY_KIT_TABLE_SIMPLE
#define CHERRY_KIT_TABLE_SIMPLE

namespace Cherry
{
    namespace Components
    {
        class TableSimple : public Component
        {
        public:
            TableSimple(const Cherry::Identifier &id, const std::string &label, const std::vector<std::shared_ptr<Component>> &rows = {}, ImGuiTableFlags flags = 0)
                : Component(id), m_Rows(rows), m_OtherFlags(flags)
            {
                // Identifier
                SetIdentifier(id);

                // Padding
                SetProperty("cell_padding_x_header", "6.0");
                SetProperty("cell_padding_y_header", "6.0");
                SetProperty("cell_padding_x_row", "6.0");
                SetProperty("cell_padding_y_row", "6.0");

                // Visibility
                SetProperty("header_visible", "true");

                // Columns
                SetProperty("columns_number", "2");
                SetProperty("columns_name_0", "First");
                SetProperty("columns_name_1", "Second");

                // Label
                SetProperty("label", label);

                SetData("renderedColumn", "-1");
            }

            void Render() override
            {
                ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | m_OtherFlags;

                // ImGuiStyle style = CherryGUI::GetStyle();
                // m_HeaderCellPaddingX = style.CellPadding.x;
                // m_HeaderCellPaddingY = style.CellPadding.y;
                // m_RowsCellPaddingX = style.CellPadding.x;
                // m_RowsCellPaddingY = style.CellPadding.y;

                std::string identifier = GetIdentifier().string();
                std::string Label = GetProperty("label");

                if (!identifier.empty())
                {
                    Label += "####" + identifier;
                }

                if (CherryGUI::BeginTable(Label.c_str(), std::stoi(GetProperty("columns_number")), flags))
                {
                    CherryGUI::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(std::stof(GetProperty("cell_padding_x_header")), std::stof(GetProperty("cell_padding_y_header"))));

                    for (int i = 0; i < std::stoi(GetProperty("columns_number")); ++i)
                    {
                        if (i == std::stoi(GetProperty("columns_number")) - 1)
                        {
                            std::string column_name_prop = "columns_name_" + std::to_string(i);
                            CherryGUI::TableSetupColumn(GetProperty(column_name_prop).c_str(), ImGuiTableColumnFlags_NoResize);
                        }
                        else
                        {
                            std::string column_name_prop = "columns_name_" + std::to_string(i);
                            CherryGUI::TableSetupColumn(GetProperty(column_name_prop).c_str());
                        }
                    }

                    if (GetProperty("header_visible") == "true")
                    {
                        CherryGUI::TableHeadersRow();
                    }

                    CherryGUI::PopStyleVar();

                    CherryGUI::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(std::stof(GetProperty("cell_padding_x_row")), std::stof(GetProperty("cell_padding_y_row"))));
                    for (auto row : m_Rows)
                    {
                        CherryGUI::TableNextRow();
                        for (int column = 0; column < std::stoi(GetProperty("columns_number")); column++)
                        {
                            CherryGUI::TableSetColumnIndex(column);
                            SetData("renderedColumn", std::to_string(column));
                            row->Render();
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
    }

    // End-User API
    namespace Kit
    {
        inline std::shared_ptr<Component> TableSimple(const std::string &label, const std::vector<std::shared_ptr<Component>> &rows = {}, ImGuiTableFlags flags = 0)
        {
            // Inline component
            /*auto table = Application::CreateAnonymousComponent<Components::TableSimple>(Components::TableSimple(anonymous_id, label, rows, flags));

            Cherry::PushParentComponent(table); // Can be a parent of other component, so we declare it
            table->RefreshContextProperties();
            table->Render();
            Cherry::PopParentComponent();

            return table->GetData("isClicked") == "true" ? true : false;*/

            auto anonymous_id = Application::GenerateUniqueID(label, rows, flags);
            auto existing = Application::GetAnonymousComponent(anonymous_id);
            if (existing)
            {
                Cherry::PushParentComponent(existing); // Can be a parent of other component, so we declare it
                existing->RefreshContextProperties();
                existing->Render();
                return existing;
            }
            else
            {

                auto button = Application::CreateAnonymousComponent<Components::TableSimple>(Components::TableSimple(anonymous_id, label, rows, flags));
                Cherry::PushParentComponent(button); // Can be a parent of other component, so we declare it
                button->RefreshContextProperties();
                button->Render();
                return button;
            }
        }

        inline std::shared_ptr<Component> TableSimple(const Cherry::Identifier &identifier, const std::string &label, const std::vector<std::shared_ptr<Component>> &rows = {}, ImGuiTableFlags flags = 0)
        {
            // Get the object if exist
            auto existing_table = Application::GetComponent(identifier);
            if (existing_table)
            {
                Cherry::PushParentComponent(existing_table); // Can be a parent of other component, so we declare it
                existing_table->RefreshContextProperties();
                existing_table->Render();
                Cherry::PopParentComponent();
                return existing_table;
            }
            else
            {
                // Create the object if not exist
                auto new_table = Application::CreateComponent<Components::TableSimple>(Components::TableSimple(identifier, label, rows, flags));

                Cherry::PushParentComponent(new_table); // Can be a parent of other component, so we declare it
                new_table->RefreshContextProperties();
                new_table->Render();
                Cherry::PopParentComponent();
                return new_table;
            }
        }
    }

}

#endif // CHERRY_KIT_TABLE_SIMPLE
