#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_MULTICHILD_TABS
#define UIKIT_V1_MULTICHILD_TABS

// UIKIT Components
#include "../../../../components/buttons/custom_buttons.hpp"
#include "../../../../components/buttons/text_buttons.hpp"
#include "../../../../components/titles/h1.hpp"
#include "../../../../components/titles/h2.hpp"
#include "../../../../components/titles/h3.hpp"
#include "../../../../components/titles/h4.hpp"
#include "../../../../components/titles/h5.hpp"
#include "../../../../components/titles/h6.hpp"
#include "../../../../components/texts/simple_text.hpp"
#include "../../../../components/combos/simple_combo.hpp"
#include "../../../../components/headers/simple_collapsing.hpp"
#include "../../../../components/headers/custom_collapsing.hpp"
#include "../../../../components/inputs/keyval/keyval_double.hpp"
#include "../../../../components/lists/custom_lists.hpp"

namespace UIKit
{
    struct Child
    {
        std::function<void()> m_Child;
        std::string m_Name;
        float m_DefaultSize;
        float m_MinSize;
        float m_MaxSize;
        bool m_Resizable = true;

        Child(const std::string& name, const std::function<void()>& child, const float& default_size = 0.0f, const float& min_size = 0.0f, const float& max_size = 0.0f) :
        m_Name(name),
        m_Child(child),
        m_DefaultSize(default_size),
        m_MinSize(min_size),
        m_MaxSize(max_size)
        {}
        
    };

    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class MultiChildTabs
    {
    public:
        MultiChildTabs(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        MultiChildTabs(const std::string &name);

        void AddChild(const Child& child);
        void RemoveChild(const std::string &child_name);
        std::function<void()> GetChild(const std::string &child_name);
        void RefreshRender(const std::shared_ptr<MultiChildTabs>& instance);

        std::vector<Child> m_Childs;

        bool m_IsHorizontal = true;

        std::string m_SelectedChildName;
        std::shared_ptr<UIKit::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_MULTICHILD_TABS