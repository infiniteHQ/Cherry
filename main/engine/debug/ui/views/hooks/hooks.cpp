#include "hooks.hpp"
#ifdef CHERRY_DEBUG

#include "../../../../app/app.hpp"

static std::shared_ptr<Cherry::Hook> s_SelectedHook = nullptr;

static const ImVec4 kBgDark = { 0.07f, 0.07f, 0.07f, 1.0f };
static const ImVec4 kBgPanel = { 0.10f, 0.10f, 0.10f, 1.0f };
static const ImVec4 kBgHover = { 0.14f, 0.14f, 0.14f, 1.0f };
static const ImVec4 kBgSelected = { 0.16f, 0.22f, 0.22f, 1.0f };
static const ImVec4 kAccentBlue = { 0.38f, 0.67f, 1.00f, 1.0f };
static const ImVec4 kAccentGreen = { 0.38f, 0.90f, 0.55f, 1.0f };
static const ImVec4 kAccentYellow = { 1.00f, 0.80f, 0.35f, 1.0f };
static const ImVec4 kAccentOrange = { 1.00f, 0.55f, 0.20f, 1.0f };
static const ImVec4 kAccentPurple = { 0.72f, 0.45f, 1.00f, 1.0f };
static const ImVec4 kTextMuted = { 0.45f, 0.45f, 0.52f, 1.0f };
static const ImVec4 kTextNormal = { 0.82f, 0.82f, 0.88f, 1.0f };
static const ImVec4 kSeparator = { 0.18f, 0.18f, 0.18f, 1.0f };

HooksView::HooksView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetInternalPaddingX(0.0f);
  m_AppWindow->SetInternalPaddingY(0.0f);
}

std::shared_ptr<Cherry::AppWindow> &HooksView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<HooksView> HooksView::Create(const std::string &name) {
  auto instance = std::shared_ptr<HooksView>(new HooksView(name));
  instance->SetupRenderCallback();
  return instance;
}

void HooksView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self)
      self->Render();
  });
}

static void RenderKVTable(const char *table_id, const std::unordered_map<std::string, std::string> &map, ImVec4 key_color) {
  if (map.empty()) {
    ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
    ImGui::TextUnformatted("  No entries");
    ImGui::PopStyleColor();
    return;
  }

  constexpr ImGuiTableFlags flags =
      ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoPadOuterX;

  ImGui::PushStyleColor(ImGuiCol_TableBorderLight, kSeparator);
  ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.f, 0.f, 0.f, 0.f));
  ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(1.f, 1.f, 1.f, 0.025f));

  if (ImGui::BeginTable(table_id, 2, flags)) {
    ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 140.f);
    ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

    for (auto const &[key, val] : map) {
      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::PushStyleColor(ImGuiCol_Text, key_color);
      ImGui::TextUnformatted(key.c_str());
      ImGui::PopStyleColor();

      ImGui::TableSetColumnIndex(1);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextNormal);
      ImGui::TextUnformatted(val.c_str());
      ImGui::PopStyleColor();
    }
    ImGui::EndTable();
  }
  ImGui::PopStyleColor(3);
}

static bool RenderCollapsibleSection(const char *label, ImVec4 dot_color, bool *p_open, auto render_fn) {
  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImVec2 row_p = ImGui::GetCursorScreenPos();
  float row_w = ImGui::GetContentRegionAvail().x;
  float row_h = ImGui::GetTextLineHeightWithSpacing() + 6.f;

  if (ImGui::IsMouseHoveringRect(row_p, ImVec2(row_p.x + row_w, row_p.y + row_h)))
    dl->AddRectFilled(row_p, ImVec2(row_p.x + row_w, row_p.y + row_h), IM_COL32(255, 255, 255, 8));

  float h = ImGui::GetTextLineHeight();
  dl->AddCircleFilled(ImVec2(row_p.x + 8, row_p.y + h * 0.5f + 4), 3.5f, ImGui::ColorConvertFloat4ToU32(dot_color));

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(*p_open ? "v" : ">");
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 4);

  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(label);
  ImGui::PopStyleColor();

  ImGui::SetCursorScreenPos(row_p);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
  std::string btn_id = std::string("##hdr_") + label;
  if (ImGui::InvisibleButton(btn_id.c_str(), ImVec2(row_w, row_h)))
    *p_open = !(*p_open);
  ImGui::PopStyleColor(3);

  if (*p_open) {
    ImGui::Spacing();
    render_fn();
    ImGui::Spacing();
    ImGui::Spacing();
  }

  ImVec2 sep_p = ImGui::GetCursorScreenPos();
  dl->AddLine(
      ImVec2(sep_p.x + 8, sep_p.y),
      ImVec2(sep_p.x + ImGui::GetContentRegionAvail().x - 8, sep_p.y),
      ImGui::ColorConvertFloat4ToU32(kSeparator));
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 8);

  return *p_open;
}

void HooksView::RenderHookDetails(std::shared_ptr<Cherry::Hook> &hook) {
  if (!hook)
    return;

  ImDrawList *dl = ImGui::GetWindowDrawList();

  static bool s_OpenInfo = true;
  static bool s_OpenData = true;
  static bool s_OpenExecution = true;

  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09f, 0.09f, 0.12f, 1.0f));
    ImGui::BeginChild("##hook_id_bar", ImVec2(0, 44), false);
    ImGui::SetCursorPos(ImVec2(12, 8));

    bool is_frame = (hook->GetExecuteOn() == Cherry::ExecuteHookOn::Frame);
    const char *exec_label_text = is_frame ? " Frame " : " Second ";
    ImVec4 exec_color = is_frame ? kAccentBlue : kAccentOrange;
    ImU32 exec_badge_col = is_frame ? IM_COL32(40, 80, 160, 80) : IM_COL32(160, 90, 20, 80);

    ImVec2 ep = ImGui::GetCursorScreenPos();
    ImVec2 esz = ImGui::CalcTextSize(exec_label_text);
    dl->AddRectFilled(ImVec2(ep.x, ep.y - 2), ImVec2(ep.x + esz.x, ep.y + esz.y + 2), exec_badge_col, 4.f);
    ImGui::PushStyleColor(ImGuiCol_Text, exec_color);
    ImGui::TextUnformatted(exec_label_text);
    ImGui::PopStyleColor();

    std::string id_str = hook->GetIdentifier().string();
    if (!id_str.empty()) {
      ImGui::SameLine(0, 10);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
      ImGui::TextUnformatted("#");
      ImGui::PopStyleColor();
      ImGui::SameLine(0, 3);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextNormal);
      ImGui::TextUnformatted(id_str.c_str());
      ImGui::PopStyleColor();
    }

    std::string cond_label = "no condition";
    if (hook->HaveCondition()) {
      cond_label = "WITH condition";
    }

    ImVec4 cond_color = kTextMuted;
    float cw = ImGui::CalcTextSize(cond_label.c_str()).x;
    ImGui::SameLine(ImGui::GetWindowWidth() - cw - 12);
    ImGui::PushStyleColor(ImGuiCol_Text, cond_color);
    ImGui::TextUnformatted(cond_label.c_str());
    ImGui::PopStyleColor();

    ImGui::EndChild();
    ImGui::PopStyleColor();
  }

  {
    ImVec2 lp = ImGui::GetCursorScreenPos();
    dl->AddLine(lp, ImVec2(lp.x + ImGui::GetContentRegionAvail().x, lp.y), IM_COL32(60, 100, 180, 120));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
  }

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 5));

  RenderCollapsibleSection("Info", kAccentBlue, &s_OpenInfo, [&]() {
    constexpr ImGuiTableFlags tflags = ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoPadOuterX;

    ImGui::PushStyleColor(ImGuiCol_TableBorderLight, kSeparator);
    ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(1, 1, 1, 0.025f));

    if (ImGui::BeginTable("##hook_info", 2, tflags)) {
      ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 140.f);
      ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::PushStyleColor(ImGuiCol_Text, kAccentBlue);
      ImGui::TextUnformatted("Identifier");
      ImGui::PopStyleColor();
      ImGui::TableSetColumnIndex(1);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextNormal);
      ImGui::TextUnformatted(hook->GetIdentifier().string().c_str());
      ImGui::PopStyleColor();

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::PushStyleColor(ImGuiCol_Text, kAccentBlue);
      ImGui::TextUnformatted("Execute on");
      ImGui::PopStyleColor();
      ImGui::TableSetColumnIndex(1);
      bool is_frame = (hook->GetExecuteOn() == Cherry::ExecuteHookOn::Frame);
      ImGui::PushStyleColor(ImGuiCol_Text, is_frame ? kAccentBlue : kAccentOrange);
      ImGui::TextUnformatted(is_frame ? "Frame" : "Second");
      ImGui::PopStyleColor();

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::PushStyleColor(ImGuiCol_Text, kAccentBlue);
      ImGui::TextUnformatted("Condition");
      ImGui::PopStyleColor();
      ImGui::TableSetColumnIndex(1);

      bool have_cond = hook->HaveCondition();
      ImGui::PushStyleColor(ImGuiCol_Text, have_cond ? kAccentGreen : kTextMuted);
      ImGui::TextUnformatted(have_cond ? "Set" : "None");
      ImGui::PopStyleColor();

      ImGui::TableNextRow();
      ImGui::TableSetColumnIndex(0);
      ImGui::PushStyleColor(ImGuiCol_Text, kAccentBlue);
      ImGui::TextUnformatted("Data entries");
      ImGui::PopStyleColor();
      ImGui::TableSetColumnIndex(1);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextNormal);
      ImGui::Text("%d", (int)hook->GetDataMap().size());
      ImGui::PopStyleColor();

      ImGui::EndTable();
    }
    ImGui::PopStyleColor(3);
  });

  RenderCollapsibleSection("Execution", kAccentOrange, &s_OpenExecution, [&]() {
    bool is_frame = (hook->GetExecuteOn() == Cherry::ExecuteHookOn::Frame);

    ImVec2 card_p = ImGui::GetCursorScreenPos();
    float card_w = ImGui::GetContentRegionAvail().x - 8.f;
    float card_h = ImGui::GetTextLineHeightWithSpacing() * 2.2f;

    ImU32 card_col = is_frame ? IM_COL32(38, 68, 130, 60) : IM_COL32(130, 80, 20, 60);
    dl->AddRectFilled(card_p, ImVec2(card_p.x + card_w, card_p.y + card_h), card_col, 4.f);
    dl->AddRect(
        card_p,
        ImVec2(card_p.x + card_w, card_p.y + card_h),
        is_frame ? IM_COL32(60, 120, 220, 80) : IM_COL32(220, 140, 40, 80),
        4.f,
        0,
        1.f);

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY() + card_h * 0.18f));
    ImGui::PushStyleColor(ImGuiCol_Text, is_frame ? kAccentBlue : kAccentOrange);
    ImGui::TextUnformatted(is_frame ? "Every Frame" : "Every Second");
    ImGui::PopStyleColor();

    ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 10, ImGui::GetCursorPosY()));
    ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
    ImGui::TextUnformatted(
        is_frame ? "Hook runs once per rendered frame (~FPS rate)" : "Hook runs once per second (timer-based)");
    ImGui::PopStyleColor();

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + card_h * 0.1f + 4.f);
    ImGui::Dummy(ImVec2(0, 0));
  });

  RenderCollapsibleSection(
      "Data", kAccentGreen, &s_OpenData, [&]() { RenderKVTable("##hook_data", hook->GetDataMap(), kAccentGreen); });

  ImGui::PopStyleVar();
}

static void RenderHookGroup(
    const char *group_label,
    bool *p_group_open,
    const std::vector<std::pair<Cherry::Identifier, std::shared_ptr<Cherry::Hook>>> &hooks_vec,
    ImDrawList *dl,
    float list_w) {
  ImVec2 hdr_p = ImGui::GetCursorScreenPos();
  float hdr_h = ImGui::GetTextLineHeightWithSpacing() + 6.f;

  if (ImGui::IsMouseHoveringRect(hdr_p, ImVec2(hdr_p.x + list_w, hdr_p.y + hdr_h)))
    dl->AddRectFilled(hdr_p, ImVec2(hdr_p.x + list_w, hdr_p.y + hdr_h), IM_COL32(255, 255, 255, 6));

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(*p_group_open ? "v" : ">");
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 4);

  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(group_label);
  ImGui::PopStyleColor();

  char cnt[16];
  snprintf(cnt, sizeof(cnt), "%d", (int)hooks_vec.size());
  float cnt_w = ImGui::CalcTextSize(cnt).x + 8.f;
  ImGui::SameLine(list_w - cnt_w - 4);
  ImVec2 bp = ImGui::GetCursorScreenPos();
  float bh = ImGui::GetTextLineHeight();
  dl->AddRectFilled(ImVec2(bp.x, bp.y), ImVec2(bp.x + cnt_w, bp.y + bh + 2), IM_COL32(60, 60, 70, 180), 3.f);
  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(cnt);
  ImGui::PopStyleColor();

  ImGui::SetCursorScreenPos(hdr_p);
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
  std::string btn_id = std::string("##grp_") + group_label;
  if (ImGui::InvisibleButton(btn_id.c_str(), ImVec2(list_w, hdr_h)))
    *p_group_open = !(*p_group_open);
  ImGui::PopStyleColor(3);

  ImVec2 sep = ImGui::GetCursorScreenPos();
  dl->AddLine(ImVec2(sep.x, sep.y), ImVec2(sep.x + list_w, sep.y), ImGui::ColorConvertFloat4ToU32(kSeparator));
  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 1);

  if (!*p_group_open)
    return;

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

  if (hooks_vec.empty()) {
    ImGui::SetCursorPosX(14);
    ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
    ImGui::TextUnformatted("No hooks");
    ImGui::PopStyleColor();
  }

  for (auto const &[id, hook] : hooks_vec) {
    if (!hook)
      continue;

    bool selected = (s_SelectedHook == hook);
    std::string label = id.string();
    if (label.empty())
      label = "unnamed";

    ImGui::PushID(hook.get());

    ImVec2 row_p = ImGui::GetCursorScreenPos();
    float row_w = ImGui::GetContentRegionAvail().x;
    float row_h = ImGui::GetTextLineHeightWithSpacing() + 6;

    if (selected) {
      dl->AddRectFilled(row_p, ImVec2(row_p.x + row_w, row_p.y + row_h), ImGui::ColorConvertFloat4ToU32(kBgSelected));
      bool is_frame = (hook->GetExecuteOn() == Cherry::ExecuteHookOn::Frame);
      dl->AddRectFilled(
          row_p, ImVec2(row_p.x + 2, row_p.y + row_h), is_frame ? IM_COL32(98, 170, 255, 255) : IM_COL32(255, 165, 60, 255));
    }

    {
      bool is_frame = (hook->GetExecuteOn() == Cherry::ExecuteHookOn::Frame);
      float dot_x = row_p.x + row_w - 10.f;
      float dot_y = row_p.y + row_h * 0.5f;
      dl->AddCircleFilled(ImVec2(dot_x, dot_y), 3.f, is_frame ? IM_COL32(80, 140, 255, 180) : IM_COL32(255, 150, 50, 180));
    }

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 14);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 3));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, kBgHover);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, kBgSelected);
    ImGui::PushStyleColor(ImGuiCol_Header, selected ? kBgSelected : ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Text, selected ? kAccentBlue : kTextNormal);

    if (ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns, ImVec2(row_w - 20, row_h)))
      s_SelectedHook = hook;

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar();
    ImGui::PopID();
  }

  ImGui::PopStyleVar();
}

void HooksView::Render() {
  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.f);

  const float list_w = 200.f;
  static bool s_GroupHooksOpen = true;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, kBgPanel);
  ImGui::BeginChild("##hooks_list", ImVec2(list_w, 0), false);

  {
    ImVec2 lp = ImGui::GetCursorScreenPos();
    dl->AddLine(lp, ImVec2(lp.x + list_w, lp.y), IM_COL32(60, 100, 180, 120));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
  }

  std::vector<std::pair<Cherry::Identifier, std::shared_ptr<Cherry::Hook>>> all_hooks(
      Cherry::GetHooks().begin(), Cherry::GetHooks().end());

  std::sort(
      all_hooks.begin(), all_hooks.end(), [](auto const &a, auto const &b) { return a.first.string() < b.first.string(); });

  RenderHookGroup("Hooks", &s_GroupHooksOpen, all_hooks, dl, list_w);

  // Other groops here if needed

  ImGui::EndChild();
  ImGui::PopStyleColor();

  {
    ImVec2 wp = ImGui::GetWindowPos();
    dl->AddLine(
        ImVec2(wp.x + list_w, wp.y),
        ImVec2(wp.x + list_w, wp.y + ImGui::GetWindowHeight()),
        ImGui::ColorConvertFloat4ToU32(kSeparator),
        1.f);
  }

  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_ChildBg, kBgDark);
  ImGui::BeginChild("##hooks_details", ImVec2(0, 0), false);

  if (s_SelectedHook) {
    RenderHookDetails(s_SelectedHook);
  } else {
    ImVec2 avail = ImGui::GetContentRegionAvail();
    ImGui::SetCursorPos(ImVec2(avail.x * 0.5f - 70, avail.y * 0.45f));
    ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
    ImGui::TextUnformatted("← Select a hook");
    ImGui::PopStyleColor();
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::PopStyleVar(4);
}

#endif  // CHERRY_DEBUG