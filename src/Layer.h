#include <string>

#pragma once

namespace UIKit {

	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(float ts) {}
		virtual void OnUIRender() {}

		bool initialized = false;

		std::string ParentWindow = "base";
	};

}