#pragma once

#include "KeyCodes.h"

#include "../../../../lib/glm/glm/glm.hpp"

namespace UIKit {

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode, const std::string& winname);
		static bool IsMouseButtonDown(MouseButton button, const std::string& winname);

		static glm::vec2 GetMousePosition(const std::string& winname);

		static void SetCursorMode(CursorMode mode, const std::string& winname);
	};

}
