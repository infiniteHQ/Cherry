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

namespace Cherry
{
    // This window can be a "subappwindow" of a parent if you use the constructor with parent parameter.
    class MultiChildTabs : public std::enable_shared_from_this<MultiChildTabs>
    {
    public:
        MultiChildTabs(const std::string &name, const std::shared_ptr<AppWindow> &parent);
        MultiChildTabs(const std::string &name);
        std::shared_ptr<Cherry::AppWindow> &GetAppWindow();

        void AddChild(const std::string &child_name, const std::function<void()> &child);
        void RemoveChild(const std::string &child_name);
        std::function<void()> GetChild(const std::string &child_name);

        static std::shared_ptr<MultiChildTabs> Create(const std::string &name);
        void SetupRenderCallback();
        void Render();

        std::unordered_map<std::string, std::function<void()>> m_Childs;

        std::string m_SelectedChildName;
        std::shared_ptr<Cherry::AppWindow> m_AppWindow;
    };
}

#endif // UIKIT_V1_MULTICHILD_TABS