#include "app_headless.hpp"

#include "../../../src/core/log.hpp"

#include <iostream>
#include <chrono>

extern bool g_ApplicationRunning;

static Cherry::Application *s_Instance = nullptr;

#ifndef APP_HEADLESS_HPP
#define APP_HEADLESS_HPP

namespace Cherry
{

	Application::Application(const ApplicationSpecification &specification)
		: m_Specification(specification)
	{
		s_Instance = this;

		Init();
	}

	Application::~Application()
	{
		Shutdown();

		s_Instance = nullptr;
	}

	Application &Application::Get()
	{
		return *s_Instance;
	}

	void Application::Init()
	{
		// Intialize logging
		Log::Init();
	}

	void Application::Shutdown()
	{
		for (auto &layer : m_LayerStack)
			layer->OnDetach();

		m_LayerStack.clear();

		g_ApplicationRunning = false;

		Log::Shutdown();
	}

	static void PushStyle()
	{
		ImGuiStyle &style = ImGui::GetStyle();
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 11.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 11.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15.0f, 6.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(9.0f, 3.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 7.0f);
	}

	static void PopStyle()
	{
		ImGui::PopStyleVar(8);
	}

	void Application::Run()
	{
		m_Running = true;

		// Main loop
		while (m_Running)
		{
			PushStyle();
			for (auto &layer : m_LayerStack)
				layer->OnUpdate(m_TimeStep);

			if (m_Specification.SleepDuration > 0.0f)
				std::this_thread::sleep_for(std::chrono::milliseconds(m_Specification.SleepDuration));

			float time = GetTime();
			m_FrameTime = time - m_LastFrameTime;
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;
			PopStyle();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	float Application::GetTime()
	{
		return m_AppTimer.Elapsed();
	}

}
#endif