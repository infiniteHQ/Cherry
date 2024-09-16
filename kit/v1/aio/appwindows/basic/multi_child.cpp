#include "./multi_child.hpp"

namespace UIKit
{
    MultiChildAppWindow::MultiChildAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");
        m_AppWindow->SetParent(parent);
        m_AppWindow->SetRenderCallback([this]() {

        });

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;

        m_AppWindow->SetLeftMenubarCallback([]() {});

        m_AppWindow->SetRightMenubarCallback([win]() {});
    }

    MultiChildAppWindow::MultiChildAppWindow(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");

        this->AddChild("Buttons", [this]()
                       {

                        ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
        if (ImGui::BeginTable("table_context_menu_2", 4, flags2))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Description");
            ImGui::TableSetupColumn("Preview");
            ImGui::TableSetupColumn("Code");

            // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
            ImGui::TableHeadersRow();
            for (int row = 0; row < 6; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if(row == 0)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Simple Image button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
                            ImageButtonSimple("Simple", "/usr/local/include/Vortex/imgs/vortex.png"); 
                        }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 1)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Low profile Image button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
    ImageButtonLowProfile("Low", "/usr/local/include/Vortex/imgs/vortex.png"); 
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 2)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Custom simple button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
    CustomButtonSimple("Simple"); 
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 3)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Custom low profile button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
    CustomButtonLowProfile("Low", "/usr/local/include/Vortex/imgs/vortex.png");
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 4)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Danger low profile button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
    DangerButtonLowProfile("Delete"); 
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 5)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Validation low profile button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
    ValidationButtonLowProfile("Continue"); 
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    
                }
            
            }
            ImGui::EndTable();
        } });

        this->AddChild("Texts", [this]()
                       {
                           ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
                           if (ImGui::BeginTable("table_context_menu_2", 4, flags2))
                           {
                               ImGui::TableSetupColumn("Name");
                               ImGui::TableSetupColumn("Description");
                               ImGui::TableSetupColumn("Preview");
                               ImGui::TableSetupColumn("Code");

                               // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
                               ImGui::TableHeadersRow();
                               for (int row = 0; row < 4; row++)
                               {
                                   ImGui::TableNextRow();
                                   for (int column = 0; column < 4; column++)
                                   {
                                       ImGui::TableSetColumnIndex(column);
                                       if (row == 0)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("Left centred text");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               SimpleTextLeft("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 1)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("Centred text");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               SimpleTextCentred("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 2)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("Right centred text");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               SimpleTextRight("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                   }
                               }
                               ImGui::EndTable();
                           }
                       });

        this->AddChild("Titles", [this]()
                       {
                           ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
                           if (ImGui::BeginTable("table_context_menu_2", 4, flags2))
                           {
                               ImGui::TableSetupColumn("Name");
                               ImGui::TableSetupColumn("Description");
                               ImGui::TableSetupColumn("Preview");
                               ImGui::TableSetupColumn("Code");

                               // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
                               ImGui::TableHeadersRow();
                               for (int row = 0; row < 11; row++)
                               {
                                   ImGui::TableNextRow();
                                   for (int column = 0; column < 4; column++)
                                   {
                                       ImGui::TableSetColumnIndex(column);
                                       if (row == 0)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H1 title");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleOne("Simple");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 1)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H1 title (colored)");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleOneColored("Simple", "#B1FF31FF");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 2)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H2 title");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleTwo("Simple");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 3)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H2 title (colored)");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleTwoColored("Simple", "#B1FF31FF");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }

                                       else if (row == 4)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H3 title");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleThree("Simple");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 5)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H3 title (colored)");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleThreeColored("Simple", "#B1FF31FF");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }

                                       else if (row == 6)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H4 title");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleFour("Simple");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 7)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H4 title (colored)");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleFourColored("Simple", "#B1FF31FF");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }

                                       else if (row == 8)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H5 title");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleFive("Simple");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 9)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H5 title (colored)");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleFiveColored("Simple", "#B1FF31FF");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }

                                       else if (row == 10)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H6 title");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleSix("Simple");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if (row == 11)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("H6 title (colored)");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               TitleSixColored("Simple", "#B1FF31FF");
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                   }
                               }
                               ImGui::EndTable();
                           } });
        this->AddChild("Keyvals", [this]() {

        });

        this->AddChild("Tables", [this]() {

        });

        this->AddChild("Grids", [this]() {

        });

        this->AddChild("Menus", [this]() {

        });

        this->AddChild("Lists", [this]() {

        });

        this->AddChild("Radios", [this]() {

        });

        this->AddChild("Selectables", [this]() {

        });

        this->AddChild("Headers", [this]()
                       {
                           ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
                           if (ImGui::BeginTable("table_context_menu_2", 4, flags2))
                           {
                               ImGui::TableSetupColumn("Name");
                               ImGui::TableSetupColumn("Description");
                               ImGui::TableSetupColumn("Preview");
                               ImGui::TableSetupColumn("Code");

                               // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
                               ImGui::TableHeadersRow();
                               for (int row = 0; row < 4; row++)
                               {
                                   ImGui::TableNextRow();
                                   for (int column = 0; column < 4; column++)
                                   {
                                       ImGui::TableSetColumnIndex(column);
                                       if (row == 0)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("Collapsing Header");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               SimpleCollapsingHeader("Test", []()
                                                                      { TitleOne("Content"); });
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                   }
                               }
                               ImGui::EndTable();
                           }
                       });

        this->AddChild("Combos", [this]()
                       {
                           ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
                           if (ImGui::BeginTable("table_context_menu_2", 4, flags2))
                           {
                               ImGui::TableSetupColumn("Name");
                               ImGui::TableSetupColumn("Description");
                               ImGui::TableSetupColumn("Preview");
                               ImGui::TableSetupColumn("Code");

                               // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
                               ImGui::TableHeadersRow();
                               for (int row = 0; row < 4; row++)
                               {
                                   ImGui::TableNextRow();
                                   for (int column = 0; column < 4; column++)
                                   {
                                       ImGui::TableSetColumnIndex(column);
                                       if (row == 0)
                                       {
                                           if (column == 0)
                                           {
                                               ImGui::Text("Simple Combo");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               SimpleCombo("Test", {"My first item", "My second item", "My third item"}, 1);
                                           }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                   }
                               }
                               ImGui::EndTable();
                           }
                       });

        this->AddChild("Color Pickers", [this]() {

        });

        m_SelectedChildName = "TEst1";

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);
    }

    void MultiChildAppWindow::AddChild(const std::string &child_name, const std::function<void()> &child)
    {
        m_Childs[child_name] = child;
    }

    void MultiChildAppWindow::RemoveChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            m_Childs.erase(it);
        }
    }

    std::function<void()> MultiChildAppWindow::GetChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void MultiChildAppWindow::RefreshRender(const std::shared_ptr<MultiChildAppWindow> &instance)
    {
        m_AppWindow->SetRenderCallback([instance]()
                                       {
                                           static float leftPaneWidth = 300.0f;
                                           const float minPaneWidth = 50.0f;
                                           const float splitterWidth = 1.5f;
                                           static int selected;


                                           ImGui::BeginChild("left pane", ImVec2(leftPaneWidth, 0), true, ImGuiWindowFlags_NoBackground);


            TitleThree("Uikit Components");

        for (const auto &child : instance->m_Childs)                                           {
                                               if (child.first == instance->m_SelectedChildName)
                                               {
                                                   ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
                                               }
                                               else
                                               {
                                                   ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); 
                                               }

                                               if (TextButtonUnderline(child.first.c_str()))
                                               {
                                                   instance->m_SelectedChildName = child.first;
                                               }

                                               ImGui::PopStyleColor();
                                           }
                                           ImGui::EndChild();

                                           ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, HexToRGBA("#44444466"));
                                           ImGui::Button("splitter", ImVec2(splitterWidth, -1));
                                           ImGui::PopStyleVar();

                                           if (ImGui::IsItemHovered())
                                           {
                                               ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
                                           }

                                           if (ImGui::IsItemActive())
                                           {
                                               float delta = ImGui::GetIO().MouseDelta.x;
                                               leftPaneWidth += delta;
                                               if (leftPaneWidth < minPaneWidth)
                                                   leftPaneWidth = minPaneWidth;
                                           }

                                           ImGui::SameLine();
                                           ImGui::BeginGroup();

                                           if(!instance->m_SelectedChildName.empty())
                                           {
                                                std::function<void()> pannel_render = instance->GetChild(instance->m_SelectedChildName);
                                                if(pannel_render)
                                                {
                                                    pannel_render();
                                                }
                                           }
                                        

                                           ImGui::EndGroup(); });
    }

}
