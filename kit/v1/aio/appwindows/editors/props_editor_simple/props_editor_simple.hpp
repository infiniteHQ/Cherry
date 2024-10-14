#pragma once
#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_CONTENT_PROPS_EDITOR_SIMPLE
#define UIKIT_V1_CONTENT_PROPS_EDITOR_SIMPLE

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/lists/custom_lists.hpp"
#include "../../../../components/tables/simple_tables.hpp"
#include "../../../../components/inputs/simple/simple_inputs.hpp"
#include "../../../../components/inputs/simple/image_inputs.hpp"
#include "../../../../components/inputs/keyval/keyval_double.hpp"

namespace Cherry
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class PropsEditorSimple : public std::enable_shared_from_this<PropsEditorSimple>
    {
    public:
        PropsEditorSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        PropsEditorSimple(const std::string &name);
        PropsEditorSimple();       
        std::shared_ptr<Cherry::AppWindow> &GetAppWindow();

        static std::shared_ptr<PropsEditorSimple> Create(const std::string &name);
        void SetupRenderCallback();
        void Render();

    private:
        // Ui Components
        std::shared_ptr<SimpleTable> cp_SimpleTable;    


        std::shared_ptr<DoubleKeyValString> cp_NameInput;   
        std::shared_ptr<DoubleKeyValString> cp_DescriptionInput;   

        std::shared_ptr<ImageStringInput> cp_SearchInput;    
        std::shared_ptr<std::string> v_SearchValue; 


        // Properties Values
        std::shared_ptr<std::string> v_Name; 
        std::shared_ptr<std::string> v_Description; 

        // Renderer
        std::shared_ptr<Cherry::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_CONTENT_PROPS_EDITOR_SIMPLE