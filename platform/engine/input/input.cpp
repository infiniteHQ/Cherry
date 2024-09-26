#include "input.hpp"

#include "../app.hpp"

namespace UIKit {

	bool Input::IsKeyDown(KeyCode keycode, const std::string& winname)
	{
		// TODO with SDL
		return false;
	}

	bool Input::IsMouseButtonDown(MouseButton button, const std::string& winname)
	{
		// TODO with SDL
		return false;
	}

	glm::vec2 Input::GetMousePosition(const std::string& winname)
	{
		// TODO with SDL
		return glm::vec2();
	}

	void Input::SetCursorMode(CursorMode mode, const std::string& winname)
	{
		// TODO with SDL
	}

}