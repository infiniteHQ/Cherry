#include "Input.h"

#include "../ApplicationGUI.h"

#include "../../../../lib/glfw/include/GLFW/glfw3.h"

namespace UIKit {

	bool Input::IsKeyDown(KeyCode keycode, const std::string& winname)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle(winname);
		int state = glfwGetKey(windowHandle, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonDown(MouseButton button, const std::string& winname)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle(winname);
		int state = glfwGetMouseButton(windowHandle, (int)button);
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePosition(const std::string& winname)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle(winname);

		double x, y;
		glfwGetCursorPos(windowHandle, &x, &y);
		return { (float)x, (float)y };
	}

	void Input::SetCursorMode(CursorMode mode, const std::string& winname)
	{
		GLFWwindow* windowHandle = Application::Get().GetWindowHandle(winname);
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL + (int)mode);
	}

}