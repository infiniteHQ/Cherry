#include "../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_DEMO_APPWINDOW
#define UIKIT_V1_DEMO_APPWINDOW

// UIKIT Components
#include "../../../components/buttons/custom_buttons.hpp"
#include "../../../components/buttons/image_buttons.hpp"
#include "../../../components/buttons/danger_buttons.hpp"
#include "../../../components/buttons/validation_buttons.hpp"
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
#include "../../../components/tables/simple_tables.hpp"
#include "../../../components/lists/custom_lists.hpp"

namespace UIKit
{
    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class DemoAppWindow
    {
    public:
        DemoAppWindow(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        DemoAppWindow(const std::string &name);

        void AddChild(const std::string &child_name, const std::function<void()> &child);
        void RemoveChild(const std::string &child_name);
        std::function<void()> GetChild(const std::string &child_name);
        void RefreshRender(const std::shared_ptr<DemoAppWindow>& instance);

        std::unordered_map<std::string, std::function<void()>> m_Childs;

        std::shared_ptr<CustomButtonSimple> cp_ButtonOne;             
        std::shared_ptr<CustomButtonSimple> cp_ButtonOneColored;        
        std::shared_ptr<CustomButtonLowProfile> cp_ButtonTwo;     
        std::shared_ptr<CustomButtonLowProfile> cp_ButtonTwoColored;
        std::shared_ptr<ImageButtonSimple> cp_ButtonThree;     
        std::shared_ptr<ImageButtonSimple> cp_ButtonThreeColored;     
        std::shared_ptr<ImageButtonLowProfile> cp_ButtonFour;     
        std::shared_ptr<ImageButtonLowProfile> cp_ButtonFourColored;     
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

        std::shared_ptr<SimpleTable> cp_SimpleTable;    
        std::shared_ptr<SimpleTable> cp_SimpleTableTwo;    
        
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
        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_DEMO_APPWINDOW