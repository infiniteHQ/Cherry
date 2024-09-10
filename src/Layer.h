#include <string>
#include <functional>

#include <iostream>

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_internal.h"
#include "../lib/imgui/backends/imgui_impl_vulkan.h"
#include "../lib/glfw/include/GLFW/glfw3.h"
#include "../lib/imgui/backends/imgui_impl_sdl.h"
#include "../lib/stb-image/stb_image.h"
#include "../lib/glm/glm/glm.hpp"
#include "../lib/json/single_include/nlohmann/json.hpp"


#pragma once

namespace UIKit {

	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnFinish() {}
		virtual void OnUpdate(float ts) {}
		virtual void OnUIRender() {}
		virtual void OnUIRefresh() {}



		ImGuiDockNode* GetDockspaceNode(){
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

}