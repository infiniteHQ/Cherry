#pragma once
#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_EDITOR_TEXT_EDITOR_SIMPLE
#define UIKIT_V1_EDITOR_TEXT_EDITOR_SIMPLE

#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/image_buttons.hpp"
#include "../../../../components/buttons/image_text_buttons.hpp"
#include "../../../../components/lists/custom_lists.hpp"
#include "../../../../components/inputs/simple/simple_inputs.hpp"
#include "../../../../../../lib/imgui/misc/text_editors/text_editor.hpp"

namespace Cherry
{

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class TextEditorSimple : public std::enable_shared_from_this<TextEditorSimple>
    {
    public:
        TextEditorSimple(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        TextEditorSimple(const std::string &name);
        TextEditorSimple();

        void RefreshRender(const std::shared_ptr<TextEditorSimple> &instance);
        static std::shared_ptr<TextEditorSimple> Create(const std::string &name);
        void SetupRenderCallback();
        void Render();
        std::shared_ptr<Cherry::AppWindow> &GetAppWindow();


    private:
        // Ui Components
        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;
        std::shared_ptr<SimpleTree> cp_Tree;
        std::shared_ptr<SimpleStringInput> cp_SearchBar;   

        // Values
        std::shared_ptr<std::string> v_StringOne; 

        // Renderer
        std::shared_ptr<ImageTextButtonSimple> cp_SaveButton;

        std::shared_ptr<TextEditor> cp_TextEditor;   
        std::shared_ptr<Cherry::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_EDITOR_TEXT_EDITOR_SIMPLEf