
#include "console.hpp"
#ifdef CHERRY_DEBUG
#include "../../../../app/app.hpp"

ConsoleView::ConsoleView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetInternalPaddingX(0.0f);
  m_AppWindow->SetInternalPaddingY(0.0f);
  Init();
  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
}

std::shared_ptr<Cherry::AppWindow> &ConsoleView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<ConsoleView> ConsoleView::Create(const std::string &name) {
  auto instance = std::shared_ptr<ConsoleView>(new ConsoleView(name));
  instance->SetupRenderCallback();
  return instance;
}

void ConsoleView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self) {
      self->Render();
    }
  });
}

void ConsoleView::Render() {
#ifdef CHERRY_ENABLE_SCRIPTING
  auto &engine = Cherry::Script::GetScriptingEngine();
#endif // CHERRY_ENABLE_SCRIPTING
  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

  if (ImGui::Button("Clear")) {
    s_Entries.clear();
#ifdef CHERRY_ENABLE_SCRIPTING
    engine.ClearOutput();
#endif // CHERRY_ENABLE_SCRIPTING
  }
  ImGui::SameLine();
  ImGui::TextDisabled("|");
  ImGui::SameLine();

  int errors = 0, warns = 0;
  for (const auto &e : s_Entries) {
    if (e.level == Cherry::Log::Level::Error ||
        e.level == Cherry::Log::Level::Fatal)
      errors++;
    else if (e.level == Cherry::Log::Level::Warn)
      warns++;
  }

  const float icon_size = ImGui::GetTextLineHeight();
  const float spacing = 6.0f;

  ImVec2 p_err = ImGui::GetCursorScreenPos();
  p_err.y = p_err.y + 5.0f;
  ImGui::Dummy(ImVec2(icon_size, icon_size));

  ImVec2 center_err =
      ImVec2(p_err.x + icon_size * 0.5f, p_err.y + icon_size * 0.5f);
  draw_list->AddCircleFilled(center_err, icon_size * 0.45f,
                             IM_COL32(255, 80, 80, 255));
  draw_list->AddLine(ImVec2(center_err.x - 3, center_err.y - 3),
                     ImVec2(center_err.x + 3, center_err.y + 3), IM_COL32_WHITE,
                     1.5f);
  draw_list->AddLine(ImVec2(center_err.x + 3, center_err.y - 3),
                     ImVec2(center_err.x - 3, center_err.y + 3), IM_COL32_WHITE,
                     1.5f);

  ImGui::SameLine(0, 4);
  ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "%d", errors);

  ImGui::SameLine(0, spacing);

  ImVec2 p_warn = ImGui::GetCursorScreenPos();
  p_warn.y = p_warn.y + 5.0f;
  ImGui::Dummy(ImVec2(icon_size, icon_size));

  ImVec2 center_warn =
      ImVec2(p_warn.x + icon_size * 0.5f, p_warn.y + icon_size * 0.5f);
  ImVec2 t1(center_warn.x, center_warn.y - 6),
      t2(center_warn.x - 7, center_warn.y + 6),
      t3(center_warn.x + 7, center_warn.y + 6);
  draw_list->AddTriangleFilled(t1, t2, t3, IM_COL32(255, 210, 10, 255));

  ImGui::SameLine(0, 4);
  ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.2f, 1.0f), "%d", warns);

  ImGui::Separator();
  const float footer_height = ImGui::GetFrameHeightWithSpacing() +
                              ImGui::GetStyle().ItemSpacing.y + 12.0f;
  ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height), false,
                    ImGuiWindowFlags_HorizontalScrollbar);

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

  for (int i = 0; i < (int)s_Entries.size(); i++) {
    const auto &entry = s_Entries[i];
    ImVec4 bg_color =
        (i % 2 == 0) ? ImVec4(1, 1, 1, 0.02f) : ImVec4(0, 0, 0, 0);
    ImVec4 text_color = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    ImU32 icon_color = IM_COL32(180, 180, 180, 255);

    if (entry.level == Cherry::Log::Level::Fatal) {
      bg_color = ImVec4(0.40f, 0.02f, 0.02f, 1.0f);
      text_color = ImVec4(1.0f, 0.85f, 0.85f, 1.0f);
      icon_color = IM_COL32(255, 60, 60, 255);
    } else if (entry.level == Cherry::Log::Level::Error) {
      bg_color = ImVec4(0.22f, 0.06f, 0.06f, 1.0f);
      text_color = ImVec4(1.0f, 0.65f, 0.65f, 1.0f);
      icon_color = IM_COL32(255, 90, 90, 255);
    } else if (entry.level == Cherry::Log::Level::Warn) {
      bg_color = ImVec4(0.22f, 0.16f, 0.03f, 1.0f);
      text_color = ImVec4(1.0f, 0.92f, 0.6f, 1.0f);
      icon_color = IM_COL32(255, 210, 10, 255);
    }

    ImVec2 pos = ImGui::GetCursorScreenPos();
    float line_h = ImGui::GetTextLineHeightWithSpacing();

    draw_list->AddRectFilled(
        pos, ImVec2(pos.x + ImGui::GetContentRegionAvail().x, pos.y + line_h),
        ImGui::ColorConvertFloat4ToU32(bg_color));

    ImGui::BeginGroup();
    ImGui::Dummy(ImVec2(22, line_h));
    ImVec2 icon_pos = ImVec2(pos.x + 11, pos.y + line_h * 0.5f);

    if (entry.level == Cherry::Log::Level::Fatal) {
      draw_list->AddCircleFilled(icon_pos, 7.5f, icon_color, 8);
      draw_list->AddText(ImVec2(icon_pos.x - 2, icon_pos.y - 7), IM_COL32_WHITE,
                         "!");
    } else if (entry.level == Cherry::Log::Level::Error) {
      draw_list->AddCircleFilled(icon_pos, 6.5f, icon_color);
      draw_list->AddLine(ImVec2(icon_pos.x - 3, icon_pos.y - 3),
                         ImVec2(icon_pos.x + 3, icon_pos.y + 3), IM_COL32_WHITE,
                         1.6f);
      draw_list->AddLine(ImVec2(icon_pos.x + 3, icon_pos.y - 3),
                         ImVec2(icon_pos.x - 3, icon_pos.y + 3), IM_COL32_WHITE,
                         1.6f);
    } else if (entry.level == Cherry::Log::Level::Warn) {
      ImVec2 p1(icon_pos.x, icon_pos.y - 7), p2(icon_pos.x - 7, icon_pos.y + 6),
          p3(icon_pos.x + 7, icon_pos.y + 6);
      draw_list->AddTriangleFilled(p1, p2, p3, icon_color);
    } else {
      draw_list->AddCircle(icon_pos, 5.0f, IM_COL32(150, 150, 150, 120), 12,
                           1.0f);
    }

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Text, text_color);
    ImGui::TextUnformatted(entry.message.c_str());
    ImGui::PopStyleColor();

    if (entry.count > 1) {
      ImGui::SameLine();
      ImGui::TextDisabled("[%d]", entry.count);
    }
    ImGui::EndGroup();
  }
  ImGui::PopStyleVar();

  if (s_ScrollToBottom)
    ImGui::SetScrollHereY(1.0f);
  s_ScrollToBottom = false;
  ImGui::EndChild();
  ImGui::Separator();

  std::string current_input(s_InputBuffer);
  std::vector<CommandDef *> matches;
  if (!current_input.empty()) {
    for (auto &def : s_CommandDefs) {
      if (def.name.find(current_input) == 0) {
        matches.push_back(&def);
        if (matches.size() >= 12)
          break;
      }
    }
  }

  if (!matches.empty()) {
    ImVec2 pos = ImGui::GetCursorScreenPos();
    float line_h = ImGui::GetTextLineHeightWithSpacing();
    ImGui::SetNextWindowPos(
        ImVec2(pos.x + 40, pos.y - (line_h * (float)matches.size()) - 20));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetContentRegionAvail().x - 60, 0));

    ImGui::PushStyleColor(ImGuiCol_WindowBg,
                          ImVec4(0.07f, 0.07f, 0.07f, 0.98f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 4));

    if (ImGui::Begin("##Suggestions", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoFocusOnAppearing)) {
      for (int i = 0; i < (int)matches.size(); i++) {
        bool selected = (i == s_SelectionIdx);
        if (selected) {
          ImGui::TextColored(ImVec4(1.0f, 0.8f, 0.0f, 1.0f), ">");
        } else {
          ImGui::Dummy(ImVec2(ImGui::GetTextLineHeight(), 1));
        }
        ImGui::SameLine();

        ImGui::TextColored(ImVec4(0.4f, 0.75f, 1.0f, 1.0f), "%s",
                           matches[i]->name.c_str());
        ImGui::SameLine();
        ImGui::TextDisabled("(");
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.85f, 0.55f, 0.3f, 1.0f), "%s",
                           matches[i]->params.c_str());
        ImGui::SameLine();
        ImGui::TextDisabled(")");

        if (selected && !matches[i]->description.empty()) {
          ImGui::SameLine();
          ImGui::TextDisabled(" // %s", matches[i]->description.c_str());
        }
      }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
  }

  ImVec2 input_cursor_pos = ImGui::GetCursorScreenPos();
  input_cursor_pos.x = input_cursor_pos.x - 5;
  ImU32 term_color = IM_COL32(140, 140, 140, 160);

  draw_list->AddRect(ImVec2(input_cursor_pos.x + 6, input_cursor_pos.y + 4),
                     ImVec2(input_cursor_pos.x + 26, input_cursor_pos.y + 20),
                     term_color, 1.0f);
  draw_list->AddLine(ImVec2(input_cursor_pos.x + 10, input_cursor_pos.y + 9),
                     ImVec2(input_cursor_pos.x + 13, input_cursor_pos.y + 12),
                     term_color, 1.2f);
  draw_list->AddLine(ImVec2(input_cursor_pos.x + 10, input_cursor_pos.y + 15),
                     ImVec2(input_cursor_pos.x + 13, input_cursor_pos.y + 12),
                     term_color, 1.2f);
  draw_list->AddLine(ImVec2(input_cursor_pos.x + 15, input_cursor_pos.y + 16),
                     ImVec2(input_cursor_pos.x + 20, input_cursor_pos.y + 16),
                     term_color, 1.2f);

  ImGui::SetCursorPosX(36);
  ImGui::PushItemWidth(-1);
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0));

  auto callback = [](ImGuiInputTextCallbackData *data) -> int {
    std::vector<CommandDef *> internal_matches;
    std::string input(data->Buf);
    for (auto &def : s_CommandDefs) {
      if (!input.empty() && def.name.find(input) == 0)
        internal_matches.push_back(&def);
    }

    switch (data->EventFlag) {
    case ImGuiInputTextFlags_CallbackCompletion: {
      if (!internal_matches.empty()) {
        int idx = (s_SelectionIdx >= 0 &&
                   s_SelectionIdx < (int)internal_matches.size())
                      ? s_SelectionIdx
                      : 0;
        data->DeleteChars(0, data->BufTextLen);
        data->InsertChars(0, (internal_matches[idx]->name + "(").c_str());
        s_SelectionIdx = -1;
      }
      break;
    }
    case ImGuiInputTextFlags_CallbackHistory: {
      if (input.empty()) {
        int prev_pos = s_HistoryPos;
        if (data->EventKey == ImGuiKey_UpArrow) {
          if (s_HistoryPos == -1)
            s_HistoryPos = (int)s_CommandHistory.size() - 1;
          else if (s_HistoryPos > 0)
            s_HistoryPos--;
        } else if (data->EventKey == ImGuiKey_DownArrow) {
          if (s_HistoryPos != -1 &&
              ++s_HistoryPos >= (int)s_CommandHistory.size())
            s_HistoryPos = -1;
        }
        if (prev_pos != s_HistoryPos) {
          data->DeleteChars(0, data->BufTextLen);
          data->InsertChars(0, s_HistoryPos >= 0
                                   ? s_CommandHistory[s_HistoryPos].c_str()
                                   : "");
        }
      } else if (!internal_matches.empty()) {
        if (data->EventKey == ImGuiKey_UpArrow) {
          if (--s_SelectionIdx < 0)
            s_SelectionIdx = (int)internal_matches.size() - 1;
        } else if (data->EventKey == ImGuiKey_DownArrow) {
          if (++s_SelectionIdx >= (int)internal_matches.size())
            s_SelectionIdx = 0;
        }
      }
      break;
    }
    }
    return 0;
  };

#ifdef CHERRY_ENABLE_SCRIPTING
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 4.0f);
  ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_EnterReturnsTrue |
                                    ImGuiInputTextFlags_CallbackCompletion |
                                    ImGuiInputTextFlags_CallbackHistory;

  if (ImGui::InputTextWithHint("##ConsoleInput", "Execute Lua command...",
                               s_InputBuffer, IM_ARRAYSIZE(s_InputBuffer),
                               input_flags, callback)) {
    std::string cmd(s_InputBuffer);
    if (!cmd.empty()) {
      engine.Execute(cmd);
      s_CommandHistory.push_back(cmd);
      s_InputBuffer[0] = '\0';
      s_HistoryPos = -1;
      s_SelectionIdx = -1;
      s_ScrollToBottom = true;
    }
    ImGui::SetKeyboardFocusHere(-1);
  }

#endif // CHERRY_ENABLE_SCRIPTING

  ImGui::PopStyleColor();
  ImGui::PopItemWidth();
  ImGui::PopStyleVar(2);
}
void ConsoleView::AddLogEntry(Cherry::Log::Level level,
                              const std::string &msg) {
  if (!s_Entries.empty() && s_Entries.back().message == msg &&
      s_Entries.back().level == level) {
    s_Entries.back().count++;
  } else {
    s_Entries.push_back({level, msg, 1});
  }
  s_ScrollToBottom = true;
}

void ConsoleView::Init() {
  Cherry::Log::AddCallback(
      [this](Cherry::Log::Level level, const std::string &msg) {
        AddLogEntry(level, msg);
      });
}

#endif // CHERRY_DEBUG