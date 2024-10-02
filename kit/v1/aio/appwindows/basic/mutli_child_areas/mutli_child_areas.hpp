#include "../../../../../../platform/engine/app.hpp"

#ifndef UIKIT_V1_MULTICHILD_AREAS
#define UIKIT_V1_MULTICHILD_AREAS

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

// Horizontal or vertical areas with resizing

namespace UIKit
{
struct Child {
    std::function<void()> m_Child;
    std::string m_Name;
    float m_DefaultSize = 0.0f;
    float m_MinSize;
    float m_MaxSize;
    float m_Size = 200.0f;
    bool m_Resizable = true;
    bool m_ResizeDisabled = false;
    bool m_Initialized = false;
    bool m_InitializedSec = false;
    bool m_InitializedTh = false;

    Child(const std::string& name, const std::function<void()>& child, const float& default_size = 0.0f, const bool& resize_disabled = false, const float& min_size = 0.0f, const float& max_size = 0.0f)
        : m_Name(name),
          m_Child(child),
          m_ResizeDisabled(resize_disabled),
          m_DefaultSize(default_size),
          m_MinSize(min_size),
          m_MaxSize(max_size) {}
};

class MultiChildAreas {
public:
    MultiChildAreas(const std::string& name, const std::shared_ptr<AppWindow>& parent);
    MultiChildAreas(const std::string& name);

    void AddChild(const Child& child);
    void RemoveChild(const std::string& child_name);
    std::function<void()> GetChild(const std::string& child_name);
    void RefreshRender(const std::shared_ptr<MultiChildAreas>& instance);

    std::vector<Child> m_Childs;
    bool m_IsHorizontal = false;
    std::string m_SelectedChildName;
    std::shared_ptr<UIKit::AppWindow> m_AppWindow;
};
}

#endif // UIKIT_V1_MULTICHILD_AREAS