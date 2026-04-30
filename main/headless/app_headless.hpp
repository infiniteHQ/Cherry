//
//  app_headless.hpp
//  Headers for headless classes of application features of Cherry
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#pragma once

#include <chrono>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <main/core/logs/log.hpp>
#include <main/core/timer.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#ifndef APP_HEADLESS_HPP
#define APP_HEADLESS_HPP

namespace Cherry {
  struct ApplicationSpecification {
    std::string Name = "Devtools";
    uint32_t Width = 1600;
    uint32_t Height = 900;
    uint64_t SleepDuration = 0;
  };

  class Application {
   public:
    Application(const ApplicationSpecification &applicationSpecification = ApplicationSpecification());
    ~Application();

    static Application &Get();

    void Run();
    void SetMenubarCallback(const std::function<void()> &menubarCallback);
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

    Timer m_AppTimer;
  };

  // Implemented by CLIENT
  Application *CreateApplication(int argc, char **argv);
}  // namespace Cherry

#endif  // APP_HEADLESS_HPP