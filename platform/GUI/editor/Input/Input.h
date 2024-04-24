#pragma once

#include "KeyCodes.h"

#include "../../../../lib/glm/glm/glm.hpp"

namespace UIKit {

	class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode);
		static bool IsMouseButtonDown(MouseButton button);

		static glm::vec2 GetMousePosition();

		static void SetCursorMode(CursorMode mode);
	};

}
