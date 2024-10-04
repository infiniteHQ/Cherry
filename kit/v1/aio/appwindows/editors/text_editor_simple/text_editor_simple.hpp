#pragma once
#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_EDITOR_TEXT_EDITOR_SIMPLE
#define UIKIT_V1_EDITOR_TEXT_EDITOR_SIMPLE

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/lists/custom_lists.hpp"
#include "../../../../components/inputs/simple/simple_inputs.hpp"
#include "../../../../../../lib/imgui/misc/text_editors/text_editor.hpp"

namespace UIKit
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class TextEditorSimple
    {
    public:
        TextEditorSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        TextEditorSimple(const std::string &name);
        TextEditorSimple();

        void RefreshRender(const std::shared_ptr<TextEditorSimple> &instance);
        std::shared_ptr<UIKit::AppWindow> m_AppWindow;

    private:
        // Ui Components
        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;
        std::shared_ptr<SimpleTree> cp_Tree;
        std::shared_ptr<SimpleStringInput> cp_SearchBar;   

        // Values
        std::shared_ptr<std::string> v_StringOne; 

        // Renderer


        std::shared_ptr<TextEditor> cp_TextEditor;   
    };
}

#endif // UIKIT_V1_EDITOR_TEXT_EDITOR_SIMPLEf