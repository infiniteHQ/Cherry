#pragma once
#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CONTENT_DATA_LOGS_VIEWER
#define UIKIT_V1_CONTENT_DATA_LOGS_VIEWER

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/lists/custom_lists.hpp"
#include "../../../../components/tables/simple_tables.hpp"
#include "../../../../components/inputs/simple/simple_inputs.hpp"
#include "../../../../components/inputs/simple/image_inputs.hpp"
#include "../../../../components/inputs/keyval/keyval_double.hpp"
#include "../../../../../../lib/imgui/misc/text_editors/text_editor.hpp"


namespace UIKit
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class DataLogsViewer
    {
    public:
        DataLogsViewer(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        DataLogsViewer(const std::string &name, const std::string &path);
        DataLogsViewer();

        void RefreshRender(const std::shared_ptr<DataLogsViewer> &instance);

void LoadFileContent(const std::string &filePath)
{
    std::ifstream file(filePath);

    if (file.is_open())
    {
        std::stringstream buffer;
        buffer << file.rdbuf();
        v_Data = buffer.str();
    }
    else
    {
        v_Data = "Error: Cannot open the file!";
    }
}



    private:
        // Ui Components
        std::shared_ptr<SimpleTable> cp_SimpleTable;    


        std::shared_ptr<DoubleKeyValString> cp_NameInput;   
        std::shared_ptr<DoubleKeyValString> cp_DescriptionInput;   

        std::shared_ptr<ImageStringInput> cp_SearchInput;    
        std::shared_ptr<TextEditor> cp_TextEditor;    
        std::shared_ptr<std::string> v_SearchValue; 


        // Properties Values
        std::shared_ptr<std::string> v_Name; 
        std::shared_ptr<std::string> v_Description; 

std::string v_Data;
std::string m_FilePath;



        // Renderer
        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_CONTENT_DATA_LOGS_VIEWER