#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_DEMO_APPWINDOW
#define UIKIT_V1_DEMO_APPWINDOW

// UIKIT Components
#include "../../../components/buttons/grouped_buttons.hpp"
#include "../../../components/buttons/dropdown_buttons.hpp"
#include "../../../components/buttons/custom_buttons.hpp"
#include "../../../components/buttons/image_buttons.hpp"
#include "../../../components/buttons/image_text_buttons.hpp"
#include "../../../components/buttons/text_buttons.hpp"
#include "../../../components/titles/h1.hpp"
#include "../../../components/titles/h2.hpp"
#include "../../../components/titles/h3.hpp"
#include "../../../components/titles/h4.hpp"
#include "../../../components/titles/h5.hpp"
#include "../../../components/titles/h6.hpp"
#include "../../../components/texts/simple_text.hpp"
#include "../../../components/combos/simple_combo.hpp"
#include "../../../components/combos/custom_combo.hpp"
#include "../../../components/headers/simple_collapsing.hpp"
#include "../../../components/headers/custom_collapsing.hpp"
#include "../../../components/inputs/keyval/keyval_double.hpp"
#include "../../../components/inputs/simple/simple_inputs.hpp"
#include "../../../components/inputs/simple/image_inputs.hpp"
#include "../../../components/tables/simple_tables.hpp"
#include "../../../components/lists/custom_lists.hpp"

#include "../../../../../lib/imgui/misc/text_editors/text_editor.hpp"

namespace Cherry
{
    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class DemoAppWindow : public std::enable_shared_from_this<DemoAppWindow>
    {
    public:
        DemoAppWindow(const std::string &name);

        void AddChild(const std::string &child_name, const std::function<void()> &child);
        void RemoveChild(const std::string &child_name);
        std::function<void()> GetChild(const std::string &child_name);
        void Render();

        std::shared_ptr<Cherry::AppWindow> &GetAppWindow();

        static std::shared_ptr<DemoAppWindow> Create(const std::string &name);
        void SetupRenderCallback();

        std::unordered_map<std::string, std::function<void()>> m_Childs;

        std::shared_ptr<CustomGroupedButtons> cp_GroupedButtonOne;        
        std::shared_ptr<CustomDrowpdownButtonSimple> cp_ButtonDropdownOne;       
        std::shared_ptr<CustomDrowpdownImageButtonSimple> cp_ButtonDropdownTwo;         
        std::shared_ptr<CustomDrowpdownImageOnlyButtonSimple> cp_ButtonDropdownThree;         
        
        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;             
        std::shared_ptr<CustomButtonSimple> cp_ButtonOneColored;        
        std::shared_ptr<ImageTextButtonSimple> cp_ButtonThree;     
        std::shared_ptr<ImageTextButtonSimple> cp_ButtonThreeColored;     
        std::shared_ptr<SimpleTree> cp_ListTreeOne;     
        std::shared_ptr<CustomTree> cp_ListTreeTwo;     

        std::shared_ptr<DoubleKeyValString> cp_InputsKeyvalDoubleString;   
        std::shared_ptr<DoubleKeyValInteger> cp_InputsKeyvalDoubleInteger;      
        std::shared_ptr<DoubleKeyValBoolean> cp_InputsKeyvalDoubleBoolean;      
        std::shared_ptr<DoubleKeyValFloat> cp_InputsKeyvalDoubleFloat;      
        std::shared_ptr<DoubleKeyValDouble> cp_InputsKeyvalDoubleDouble;      
        std::shared_ptr<DoubleKeyValXYZVector> cp_InputsKeyvalDoubleXYZ;      
        std::shared_ptr<DoubleKeyValSimpleCombo> cp_InputsKeyvalDoubleSimpleCombo;    

        std::shared_ptr<SimpleStringInput> cp_InputsSimpleString;    
        std::shared_ptr<ImageStringInput> cp_InputsImageString;    

        std::shared_ptr<SimpleTable> cp_SimpleTable;    
        std::shared_ptr<SimpleTable> cp_SimpleTableTwo;    
 
        std::shared_ptr<TextEditor> cp_TextEditor;   
        
        std::shared_ptr<std::string> v_StringOne;
        std::shared_ptr<int> v_IntegerOne;
        std::shared_ptr<float> v_FloatOne;
        std::shared_ptr<bool> v_BooleanOne;
        std::shared_ptr<double> v_DoubleOne;


        std::shared_ptr<float> v_Xvalue;        
        std::shared_ptr<float> v_Yvalue;        
        std::shared_ptr<float> v_Zvalue;

        std::shared_ptr<ComboSimple> cp_ComboOne;      
        std::shared_ptr<ComboCustom> cp_ComboTwo;   

        std::string m_SelectedChildName;
        std::shared_ptr<Cherry::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_DEMO_APPWINDOW