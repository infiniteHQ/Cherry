#pragma once

#include "../../../src/layer.hpp"
#include "../../../src/timer.hpp"

#include <string>
#include <vector>
#include <memory>
#include <functional>

#ifndef APP_HEADLESS_HPP
#define APP_HEADLESS_HPP

namespace Cherry
{
	struct ApplicationSpecification
	{
		std::string Name = "Devtools";
		uint32_t Width = 1600;
		uint32_t Height = 900;

		uint64_t SleepDuration = 0;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& applicationSpecification = ApplicationSpecification());
		~Application();

		static Application& Get();

		void Run();

		// No menubar for headless apps
		void SetMenubarCallback(const std::function<void()>& menubarCallback) {}
		
		template<typename T>
		void PushLayer()
		{
			static_assert(std::is_base_of<Layer, T>::value, "Pushed type is not subclass of Layer!");
			m_LayerStack.emplace_back(std::make_shared<T>())->OnAttach();
		}

		void PushLayer(const std::shared_ptr<Layer>& layer) { m_LayerStack.emplace_back(layer); layer->OnAttach(); }

		void Close();

		float GetTime();
	private:
		void Init();
		void Shutdown();
	private:
		ApplicationSpecification m_Specification;
		bool m_Running = false;

		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;
		float m_LastFrameTime = 0.0f;

		std::vector<std::shared_ptr<Layer>> m_LayerStack;
		Timer m_AppTimer;
	};

	// Implemented by CLIENT
	Application* CreateApplication(int argc, char** argv);
}

#endif // APP_HEADLESS_HPP