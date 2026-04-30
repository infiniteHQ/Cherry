#pragma once

#include <functional>
#include <iostream>
#include <string>

#include "../../lib/glm/glm/glm.hpp"
#include "../../lib/imgui/backends/imgui_impl_vulkan.h"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/imgui_internal.h"
#include "../../lib/json/single_include/nlohmann/json.hpp"
#include "../../lib/stb-image/stb_image.h"

namespace Cherry {

  class Layer {
   public:
    virtual ~Layer() = default;

    virtual void OnAttach() {
    }
    virtual void OnDetach() {
    }

    virtual void OnFinish() {
    }
    virtual void OnUpdate(float ts) {
    }
    virtual void OnUIRender() {
    }
    virtual void OnUIRefresh() {
    }

    ImGuiDockNode* GetDockspaceNode() {
      return ImGui::GetWindowDockNode();
    }

    void ControlWindow(ImGuiWindow* win) {
      m_WindowControlCallbalck(win);
    }

    std::function<void(ImGuiWindow*)> m_WindowControlCallbalck;

    bool initialized = false;
    std::string LayerName;
    std::string ParentWindow = "base";
  };

}  // namespace Cherry