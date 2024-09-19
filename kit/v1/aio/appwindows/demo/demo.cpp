#include "./demo.hpp"

namespace UIKit
{
    DemoAppWindow::DemoAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent)
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

    DemoAppWindow::DemoAppWindow(const std::string &name)
    {
        m_AppWindow = std::make_shared<UIKit::AppWindow>(name, name);
        m_AppWindow->SetIcon("/usr/local/include/Vortex/imgs/vortex.png");

        cp_ButtonOne = Application::Get().CreateComponent<CustomButtonSimple>("button_1", "Custom Simple");
        cp_ButtonOneColored = Application::Get().CreateComponent<CustomButtonSimple>("button_1_colored", "Custom Simple", "#2424F4FF", "#2424F4FF", "#2525F5FF", "#2525F5FF", "#2626F6FF", "#2525F5FF");
        cp_ButtonTwo = Application::Get().CreateComponent<CustomButtonLowProfile>("button_2", "Custom Low Profile");
        cp_ButtonTwoColored = Application::Get().CreateComponent<CustomButtonLowProfile>("button_2_colored", "Custom Low Profile", "#2424F4FF", "#2424F4FF", "#2525F5FF", "#2525F5FF", "#2626F6FF", "#2525F5FF");
        cp_ButtonThree = Application::Get().CreateComponent<ImageButtonSimple>("button_3", "Custom Low Profile");
        cp_ButtonThreeColored = Application::Get().CreateComponent<ImageButtonSimple>("button_3_colored", "Custom Low Profile", "/usr/local/include/Vortex/imgs/vortex.png", "#2424F4FF", "#2424F4FF", "#2525F5FF", "#2525F5FF", "#2626F6FF", "#2525F5FF");
        cp_ButtonFour = Application::Get().CreateComponent<ImageButtonLowProfile>("button_4", "Custom Low Profile");
        cp_ButtonFourColored = Application::Get().CreateComponent<ImageButtonLowProfile>("button_4", "Custom Low Profile", "/usr/local/include/Vortex/imgs/vortex.png", "#2424F4FF", "#2424F4FF", "#2525F5FF", "#2525F5FF", "#2626F6FF", "#2525F5FF");
        cp_ComboOne = Application::Get().CreateComponent<ComboSimple>("combo_1", "SuperCombo", std::vector<std::string>{"My first item", "My second item", "My third item"}, 1);

        cp_ComboTwo = Application::Get().CreateComponent<ComboCustom>("combo_2", "SuperCombo2", std::vector<std::function<void()>>{[]()
                                                                                                                                   {
                                                                                                                                       static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                                                                                                                                       ImGui::Image(texture, ImVec2(15, 15));
                                                                                                                                       ImGui::SameLine();
                                                                                                                                       ImGui::Text("Icon Selected");
                                                                                                                                   },
                                                                                                                                   []()
                                                                                                                                   {
                                                                                                                                       static ImTextureID texture = Application::Get().GetCurrentRenderedWindow()->get("/usr/local/include/Vortex/imgs/vortex.png")->GetImGuiTextureID(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
                                                                                                                                       ImGui::Image(texture, ImVec2(15, 15));
                                                                                                                                       ImGui::SameLine();
                                                                                                                                       ImGui::Text("Second Selected");
                                                                                                                                   }},
                                                                      1);

        v_StringOne = std::make_shared<std::string>("Operationnal");
        v_IntegerOne = std::make_shared<int>(42);
        v_BooleanOne = std::make_shared<bool>(true);
        v_FloatOne = std::make_shared<float>(4.2f);

        v_Xvalue = std::make_shared<float>(2.27f);
        v_Yvalue = std::make_shared<float>(8.65f);
        v_Zvalue = std::make_shared<float>(6.24f);

        cp_InputsSimpleString = Application::Get().CreateComponent<SimpleStringInput>("keyvaldouble_1", v_StringOne, "Simple string value");

        cp_InputsKeyvalDoubleString = Application::Get().CreateComponent<DoubleKeyValString>("keyvaldouble_1", v_StringOne, "Simple string value");
        cp_InputsKeyvalDoubleInteger = Application::Get().CreateComponent<DoubleKeyValInteger>("keyvaldouble_2", v_IntegerOne, "Simple integer value");
        cp_InputsKeyvalDoubleBoolean = Application::Get().CreateComponent<DoubleKeyValBoolean>("keyvaldouble_3", v_BooleanOne, "Simple boolean value");
        cp_InputsKeyvalDoubleFloat = Application::Get().CreateComponent<DoubleKeyValFloat>("keyvaldouble_4", v_FloatOne, "Simple float value");
        cp_InputsKeyvalDoubleDouble = Application::Get().CreateComponent<DoubleKeyValDouble>("keyvaldouble_5", v_DoubleOne, "Simple double value");
        cp_InputsKeyvalDoubleXYZ = Application::Get().CreateComponent<DoubleKeyValXYZVector>("keyvaldouble_5", v_Xvalue, v_Yvalue, v_Zvalue, "Simple double value");
        cp_InputsKeyvalDoubleSimpleCombo = Application::Get().CreateComponent<DoubleKeyValSimpleCombo>("keyvaldouble_6", std::vector<std::string>{"My first item", "My second item", "My third item"}, 1, "Simple double value");

        cp_SimpleTable = Application::Get().CreateComponent<SimpleTable>("simpletable_1", "Simple double value", std::vector<std::string>{"One", "Two", "Three"});
        cp_SimpleTableTwo = Application::Get().CreateComponent<SimpleTable>("simpletable_2", "KeyvA", std::vector<std::string>{"Key", "Value"});

        std::vector<SimpleTree::SimpleTreeNode> node2 = {
            SimpleTree::SimpleTreeNode("Node 2", {[this]()
                                                  { ImGui::Text("Content 1 --"); },
                                                  []()
                                                  { ImGui::Text("Content 2 --"); }})};

        std::vector<SimpleTree::SimpleTreeNode> node3 = {
            SimpleTree::SimpleTreeNode("Node 3", {[this]()
                                                  { ImGui::Text("Content 1 --"); },
                                                  []()
                                                  { ImGui::Text("Content 2 --"); }})};

        std::vector<SimpleTree::SimpleTreeNode> nodes = {
            SimpleTree::SimpleTreeNode("Node 1", {[this]()
                                                  { ImGui::Text("Content 1 --"); },
                                                  []()
                                                  { ImGui::Text("Content 2 --"); }})};

        node2[0].m_Children.push_back(node3[0]);
        nodes[0].m_Children.push_back(node2[0]);

        cp_ListTreeOne = Application::Get().CreateComponent<SimpleTree>("keyvaldouble_2", nodes, 2, "SuperKeyval");

        std::vector<CustomTree::CustomTreeNode> nodesd = {
            CustomTree::CustomTreeNode({
                []()
                { ImGui::Text("Root"); }, // Column 1 content
                []()
                { ImGui::Text("--"); }, // Column 2 content
                []()
                { ImGui::Text("Folder"); } // Column 3 content
            }),
            CustomTree::CustomTreeNode({
                []()
                { ImGui::Text("File1_a.wav"); }, // Column 1 content
                []()
                { ImGui::Text("123000"); }, // Column 2 content
                []()
                { ImGui::Text("Audio file"); } // Column 3 content
            })};
        std::vector<CustomTree::CustomTreeNode> nodesd2 = {
            CustomTree::CustomTreeNode({
                []()
                { ImGui::Text("Root22"); }, // Column 1 content
                []()
                { ImGui::Text("--"); }, // Column 2 content
                []()
                { ImGui::Text("Folder"); } // Column 3 content
            }),
            CustomTree::CustomTreeNode({
                []()
                { ImGui::Text("File1_a.wav"); }, // Column 1 content
                []()
                { ImGui::Text("123000"); }, // Column 2 content
                []()
                { ImGui::Text("Audio file"); } // Column 3 content
            })};

        std::vector<CustomTree::CustomTreeNode> nodesd3 = {
            CustomTree::CustomTreeNode({
                []()
                { ImGui::Text("Root33"); }, // Column 1 content
                []()
                { ImGui::Text("--"); }, // Column 2 content
                []()
                { ImGui::Text("Folder"); } // Column 3 content
            }),
            CustomTree::CustomTreeNode({
                []()
                { ImGui::Text("File1_a.wav"); }, // Column 1 content
                []()
                { ImGui::Text("123000"); }, // Column 2 content
                []()
                { ImGui::Text("Audio file"); } // Column 3 content
            })};

        nodesd2[0].ChildNodes.push_back(nodesd3[0]);
        nodesd[0].ChildNodes.push_back(nodesd2[0]);

        cp_ListTreeTwo = Application::Get().CreateComponent<CustomTree>("keyvaldouble_2", nodesd, 3);

        this->AddChild("Buttons", [this]()
                       {

                        ImGui::ShowDemoWindow();

                        ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
        if (ImGui::BeginTable("table_context_menu_2", 4, flags2))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Description");
            ImGui::TableSetupColumn("Preview");
            ImGui::TableSetupColumn("Code");

            // [2.1] Right-click on the TableHeadersRow() line to open the default table context menu.
            ImGui::TableHeadersRow();
            for (int row = 0; row < 10; row++)
            {
                ImGui::TableNextRow();
                for (int column = 0; column < 4; column++)
                {
                    ImGui::TableSetColumnIndex(column);
                    if(row == 0)
                    {
                        if(column == 0)
                        {
                            ImGui::Text("Custom Simple Button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
                            cp_ButtonOne->Render("Normal");
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
                            ImGui::Text("Custom Low Profile Button");
                        }
                        else if(column == 1)
                        {
                            ImGui::Text("This is a incredible Simpe Image button");
                        }
                        else if(column == 2)
                        {
                            cp_ButtonOneColored->Render("Normal");
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
                            cp_ButtonTwo->Render("Normal");
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
                            cp_ButtonTwoColored->Render("Normal");
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
                            cp_ButtonThree->Render("Normal");
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
                            cp_ButtonThreeColored->Render("Normal");
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 6)
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
                            cp_ButtonFour->Render("Normal");
    }
                        else if(column == 3)
                        {
                            ImGui::Button("Copy code");
                        }
                    }
                    else if(row == 7)
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
                            cp_ButtonFourColored->Render("Normal");
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
                           } });

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
        this->AddChild("Keyvals", [this]()
                       {
                           ImGuiTableFlags flags2 = ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable | ImGuiTableFlags_Borders;
                           if (ImGui::BeginTable("table_context_menu_3", 4, flags2))
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
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qsdqsd", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255)); 
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255));  

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleString->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
                                          }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if(row == 1)
                                       {
                                          if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qs2 dqsd", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255)); 
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255));  

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleInteger->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
                                          }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       } 
                                       else if(row == 2)
                                       {
                                          if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qs2 cdfgdf", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255)); 

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleBoolean->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
                                          }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if(row == 3)
                                       {
                                          if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qs2 cdfgdf", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255)); 

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleFloat->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
                                          }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       } else if(row == 4)
                                       {
                                          if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qs2 cdfgdf", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255)); 

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleDouble->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
                                          }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }else if(row == 5)
                                       {
                                          if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qs2 cdfgdf", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255)); 

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleXYZ->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
                                          }
                                           else if (column == 3)
                                           {
                                               ImGui::Button("Copy code");
                                           }
                                       }
                                       else if(row == 6)
                                       {
                                          if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                             static ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame | ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableFlags_Borders | ImGuiTableFlags_BordersInnerV;

                                           if (ImGui::BeginTable("qs2 cdfgdf", 2, flags))
                                           {
                                               ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
                                               ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);

                                               
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, IM_COL32(0, 0, 0, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableBorderLight, IM_COL32(0, 0, 0, 255)); 

                                               ImGui::PushStyleColor(ImGuiCol_TableRowBg, IM_COL32(200, 200, 200, 255));
                                               ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, IM_COL32(200, 200, 200, 255));

                                               for (int srow = 0; srow < 1; srow++)
                                               {
                                                   ImGui::TableNextRow();
                                                   for (int scolumn = 0; scolumn < 2; scolumn++)
                                                   {
                                                       ImGui::TableSetColumnIndex(scolumn);
                                                       cp_InputsKeyvalDoubleSimpleCombo->Render(scolumn);
                                                   }
                                               }

                ImGui::PopStyleColor(4);
                ImGui::PopStyleVar();
                ImGui::EndTable();
                                           }
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

        this->AddChild("Inputs", [this]()
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
                                               cp_InputsSimpleString->Render("Normal");
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

        this->AddChild("Tables", [this]()
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
                                               cp_SimpleTable->Render(std::vector<SimpleTable::SimpleTableRow>{
                                                   SimpleTable::SimpleTableRow({[]()
                                                                                { ImGui::Text("OneRow"); },
                                                                                []()
                                                                                { ImGui::Text("OneRow"); }
                                                                                })});
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
                                               ImGui::Text("Keyval table");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {


                                                std::vector<SimpleTable::SimpleTableRow> keyvals;

                                                keyvals.push_back(SimpleTable::SimpleTableRow({[this]()
                                                                                { cp_InputsKeyvalDoubleString->Render(0); },
                                                                                [this]()
                                                                                { cp_InputsKeyvalDoubleString->Render(1); }
                                                                                }));

                                                keyvals.push_back(SimpleTable::SimpleTableRow({[this]()
                                                                                { cp_InputsKeyvalDoubleString->Render(0); },
                                                                                [this]()
                                                                                { cp_InputsKeyvalDoubleString->Render(1); }
                                                                                }));

                                                keyvals.push_back(SimpleTable::SimpleTableRow({[this]()
                                                                                { cp_InputsKeyvalDoubleInteger->Render(0); },
                                                                                [this]()
                                                                                { cp_InputsKeyvalDoubleInteger->Render(1); }
                                                                                }));

                                               cp_SimpleTableTwo->Render(keyvals);
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

        this->AddChild("Grids", [this]() {

        });

        this->AddChild("Menus", [this]() {

        });

        this->AddChild("Trees", [this]()
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
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                        cp_ListTreeOne->Render();
                                          }
                                           else if(column == 3)
                                          {
                                               ImGui::Button("Copy code");
                                          }
                                       }
                                       else if (row == 1)
                                       {
                                         if (column == 0)
                                           {
                                               ImGui::Text("String double keyval input");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if(column == 2)
                                          {
                                        cp_ListTreeTwo->Render();
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

        this->AddChild("Lists", [this]() {});
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
                                               ImGui::Text("Custom Collapsing Header Simple");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               CustomCollapsingHeaderSimple("Header", []()
                                                                      { TitleOne("Content"); });
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
                                               ImGui::Text("Custom Collapsing Header Logo");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               CustomCollapsingHeaderLogo("Header2", "/usr/local/include/Vortex/imgs/vortex.png", []()
                                                                      { TitleOne("Content"); });
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
                                               ImGui::Text("Custom Collapsing Header Logo");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               CustomCollapsingHeaderLogoCallback("Header2", "/usr/local/include/Vortex/imgs/vortex.png", 
                                               [this](){ cp_ButtonFour->Render("f"); ImGui::SameLine(); cp_ButtonFour->Render("f");},
                                            [](){ TitleOne("ccc"); });
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

        this->AddChild("Combos", [this]()
                       {
                        std::cout << Application::Get().GetComponentData("combo_1", "selected_index") << std::endl;
                        std::cout << Application::Get().GetComponentData("combo_1", "selected_string") << std::endl;
                        std::cout << Application::Get().GetComponentData("combo_1", "lastChanged") << std::endl;
                        std::cout << "Yeah : "<< UIKIT_DATA("combo_1", "lastChanged") << std::endl;
                        
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
                                               cp_ComboOne->Render("Normal");
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
                                               ImGui::Text("Simple Combo");
                                           }
                                           else if (column == 1)
                                           {
                                               ImGui::Text("This is a incredible Simpe Image button");
                                           }
                                           else if (column == 2)
                                           {
                                               cp_ComboTwo->Render("Normal");
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

        this->AddChild("Color Pickers", [this]() {

        });

        m_SelectedChildName = "TEst1";

        std::shared_ptr<UIKit::AppWindow> win = m_AppWindow;
        m_AppWindow->SetLeftMenubarCallback([]() {});
        m_AppWindow->SetRightMenubarCallback([win]() {});

        Application::Get().PutWindow(m_AppWindow);
    }

    void DemoAppWindow::AddChild(const std::string &child_name, const std::function<void()> &child)
    {
        m_Childs[child_name] = child;
    }

    void DemoAppWindow::RemoveChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            m_Childs.erase(it);
        }
    }

    std::function<void()> DemoAppWindow::GetChild(const std::string &child_name)
    {
        auto it = m_Childs.find(child_name);
        if (it != m_Childs.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void DemoAppWindow::RefreshRender(const std::shared_ptr<DemoAppWindow> &instance)
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
