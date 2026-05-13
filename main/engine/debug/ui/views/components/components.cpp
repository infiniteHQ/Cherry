//
//  components.cpp
//  Sources for devtools components inspector
//
//  Copyright (c) 2024-2026 Diego Moreno
//  Copyright (c) 2026 Infinite
//
//	This work is licensed under the terms of the MIT license.
//	For a copy, see <https://opensource.org/licenses/MIT>.
//

#include "components.hpp"
#ifdef CHERRY_DEBUG
#include "../../../../app/app.hpp"

static std::shared_ptr<Cherry::Component> s_SelectedComponent = nullptr;

static const ImVec4 kBgDark = { 0.07f, 0.07f, 0.07f, 1.0f };
static const ImVec4 kBgPanel = { 0.10f, 0.10f, 0.10f, 1.0f };
static const ImVec4 kBgHover = { 0.14f, 0.14f, 0.14f, 1.0f };
static const ImVec4 kBgSelected = { 0.16f, 0.22f, 0.22f, 1.0f };
static const ImVec4 kAccentBlue = { 0.38f, 0.67f, 1.00f, 1.0f };
static const ImVec4 kAccentGreen = { 0.38f, 0.90f, 0.55f, 1.0f };
static const ImVec4 kAccentYellow = { 1.00f, 0.80f, 0.35f, 1.0f };
static const ImVec4 kTextMuted = { 0.45f, 0.45f, 0.52f, 1.0f };
static const ImVec4 kTextNormal = { 0.82f, 0.82f, 0.88f, 1.0f };
static const ImVec4 kSeparator = { 0.18f, 0.18f, 0.18f, 1.0f };

ComponentsView::ComponentsView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetInternalPaddingX(0.0f);
  m_AppWindow->SetInternalPaddingY(0.0f);
}

std::shared_ptr<Cherry::AppWindow> &ComponentsView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<ComponentsView> ComponentsView::Create(const std::string &name) {
  auto instance = std::shared_ptr<ComponentsView>(new ComponentsView(name));
  instance->SetupRenderCallback();
  return instance;
}

void ComponentsView::SetupRenderCallback() {
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
  ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
  ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(1.0f, 1.0f, 1.0f, 0.025f));

  if (ImGui::BeginTable(table_id, 2, flags)) {
    ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 140.0f);
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
  float row_h = ImGui::GetTextLineHeightWithSpacing() + 6.0f;
  bool hovered = ImGui::IsMouseHoveringRect(row_p, ImVec2(row_p.x + row_w, row_p.y + row_h));
  if (hovered)
    dl->AddRectFilled(row_p, ImVec2(row_p.x + row_w, row_p.y + row_h), IM_COL32(255, 255, 255, 8));

  float h = ImGui::GetTextLineHeight();
  dl->AddCircleFilled(ImVec2(row_p.x + 8, row_p.y + h * 0.5f + 4), 3.5f, ImGui::ColorConvertFloat4ToU32(dot_color));

  const char *arrow = *p_open ? "V" : ">";
  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 18);
  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(arrow);
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

void ComponentsView::RenderComponentDetails(std::shared_ptr<Cherry::Component> &comp) {
  if (!comp)
    return;

  ImDrawList *dl = ImGui::GetWindowDrawList();

  static bool s_OpenParent = true;
  static bool s_OpenProps = true;
  static bool s_OpenData = true;
  static bool s_OpenCtxProps = false;
  static bool s_OpenCtxData = false;

  {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.09f, 0.09f, 0.12f, 1.0f));
    ImGui::BeginChild("##id_bar", ImVec2(0, 44), false);

    ImGui::SetCursorPos(ImVec2(12, 8));

    ImVec2 type_p = ImGui::GetCursorScreenPos();
    std::string type_label = " " + comp->GetType() + " ";
    ImVec2 type_sz = ImGui::CalcTextSize(type_label.c_str());
    dl->AddRectFilled(
        ImVec2(type_p.x, type_p.y - 2),
        ImVec2(type_p.x + type_sz.x, type_p.y + type_sz.y + 2),
        IM_COL32(60, 100, 160, 80),
        4.0f);
    ImGui::PushStyleColor(ImGuiCol_Text, kAccentBlue);
    ImGui::TextUnformatted(type_label.c_str());
    ImGui::PopStyleColor();

    std::string id_str = comp->GetIdentifier().string();
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

    const char *status_label = comp->GetIsComponentRendered() ? "live" : "hidden";
    ImVec4 status_color = comp->GetIsComponentRendered() ? kAccentGreen : kTextMuted;
    float sw = ImGui::CalcTextSize(status_label).x;
    ImGui::SameLine(ImGui::GetWindowWidth() - sw - 12);
    ImGui::PushStyleColor(ImGuiCol_Text, status_color);
    ImGui::TextUnformatted(status_label);
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

  RenderCollapsibleSection("Parent Component", kAccentYellow, &s_OpenParent, [&]() {
    bool has_parent = comp->HaveParent();

    if (!has_parent) {
      ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
      ImGui::TextUnformatted("  No parent");
      ImGui::PopStyleColor();
    } else {
      const char *parent_id = comp->GetParentIdentifier().string().c_str();
      const char *parent_type = comp->GetParentType().c_str();
      ImVec2 bp = ImGui::GetCursorScreenPos();
      std::string badge = std::string(" ") + parent_type + " ";
      ImVec2 bsz = ImGui::CalcTextSize(badge.c_str());
      dl->AddRectFilled(ImVec2(bp.x, bp.y - 2), ImVec2(bp.x + bsz.x, bp.y + bsz.y + 2), IM_COL32(160, 130, 40, 80), 4.0f);
      ImGui::PushStyleColor(ImGuiCol_Text, kAccentYellow);
      ImGui::TextUnformatted(badge.c_str());
      ImGui::PopStyleColor();

      ImGui::SameLine(0, 10);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
      ImGui::TextUnformatted("#");
      ImGui::PopStyleColor();
      ImGui::SameLine(0, 3);
      ImGui::PushStyleColor(ImGuiCol_Text, kTextNormal);
      ImGui::TextUnformatted(parent_id);
      ImGui::PopStyleColor();
    }
  });

  RenderCollapsibleSection(
      "Properties", kAccentBlue, &s_OpenProps, [&]() { RenderKVTable("##props", comp->GetPropertiesMap(), kAccentBlue); });

  RenderCollapsibleSection(
      "Data", kAccentGreen, &s_OpenData, [&]() { RenderKVTable("##data", comp->GetDataMap(), kAccentGreen); });

  RenderCollapsibleSection("Context Properties", ImVec4(0.5f, 0.6f, 0.9f, 1.0f), &s_OpenCtxProps, [&]() {
    RenderKVTable("##ctx_props", comp->GetContextPropertiesMap(), ImVec4(0.5f, 0.6f, 0.9f, 1.0f));
  });

  RenderCollapsibleSection("Context Data", ImVec4(0.9f, 0.5f, 0.9f, 1.0f), &s_OpenCtxData, [&]() {
    RenderKVTable("##ctx_data", comp->GetContextDataMap(), ImVec4(0.9f, 0.5f, 0.9f, 1.0f));
  });

  ImGui::PopStyleVar();
}

static void RenderComponentGroup(
    const char *group_label,
    bool *p_group_open,
    const std::vector<std::shared_ptr<Cherry::Component>> &comps,
    ImDrawList *dl,
    float list_w) {
  ImVec2 hdr_p = ImGui::GetCursorScreenPos();
  float hdr_h = ImGui::GetTextLineHeightWithSpacing() + 6.0f;
  bool hovered = ImGui::IsMouseHoveringRect(hdr_p, ImVec2(hdr_p.x + list_w, hdr_p.y + hdr_h));
  if (hovered)
    dl->AddRectFilled(hdr_p, ImVec2(hdr_p.x + list_w, hdr_p.y + hdr_h), IM_COL32(255, 255, 255, 6));

  ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 6);
  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(*p_group_open ? "▾" : "▸");
  ImGui::PopStyleColor();
  ImGui::SameLine(0, 4);

  ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
  ImGui::TextUnformatted(group_label);
  ImGui::PopStyleColor();

  char cnt[16];
  snprintf(cnt, sizeof(cnt), "%d", (int)comps.size());
  float cnt_w = ImGui::CalcTextSize(cnt).x + 8.0f;
  ImGui::SameLine(list_w - cnt_w - 4);
  ImVec2 bp = ImGui::GetCursorScreenPos();
  float bh = ImGui::GetTextLineHeight();
  dl->AddRectFilled(ImVec2(bp.x, bp.y), ImVec2(bp.x + cnt_w, bp.y + bh + 2), IM_COL32(60, 60, 70, 180), 3.0f);
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

  if (comps.empty()) {
    ImGui::SetCursorPosX(14);
    ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
    ImGui::TextUnformatted("No components");
    ImGui::PopStyleColor();
  }

  for (auto &comp : comps) {
    if (!comp)
      continue;

    bool selected = (s_SelectedComponent == comp);
    std::string label = comp->GetIdentifier().string();
    if (label.empty())
      label = "anonymous";

    ImGui::PushID(comp.get());

    ImVec2 row_p = ImGui::GetCursorScreenPos();
    float row_w = ImGui::GetContentRegionAvail().x;
    float row_h = ImGui::GetTextLineHeightWithSpacing() + 6;

    if (selected)
      dl->AddRectFilled(row_p, ImVec2(row_p.x + row_w, row_p.y + row_h), ImGui::ColorConvertFloat4ToU32(kBgSelected));
    if (selected)
      dl->AddRectFilled(row_p, ImVec2(row_p.x + 2, row_p.y + row_h), IM_COL32(98, 170, 255, 255));

    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 14);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 3));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, kBgHover);
    ImGui::PushStyleColor(ImGuiCol_HeaderActive, kBgSelected);
    ImGui::PushStyleColor(ImGuiCol_Header, selected ? kBgSelected : ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_Text, selected ? kAccentBlue : kTextNormal);

    if (ImGui::Selectable(label.c_str(), selected, ImGuiSelectableFlags_SpanAllColumns, ImVec2(row_w - 14, row_h)))
      s_SelectedComponent = comp;

    ImGui::PopStyleColor(4);
    ImGui::PopStyleVar();
    ImGui::PopID();
  }

  ImGui::PopStyleVar();
}

void ComponentsView::Render() {
  auto pool = CherryApp.GetComponentPool();
  if (!pool)
    return;

  ImDrawList *dl = ImGui::GetWindowDrawList();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);

  const float list_w = 200.0f;
  static bool s_GroupGeneralOpen = true;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, kBgPanel);
  ImGui::BeginChild("##list", ImVec2(list_w, 0), false);

  {
    ImVec2 lp = ImGui::GetCursorScreenPos();
    dl->AddLine(lp, ImVec2(lp.x + list_w, lp.y), IM_COL32(60, 100, 180, 120));
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
  }

  std::vector<std::shared_ptr<Cherry::Component>> all_comps;
  all_comps.insert(all_comps.end(), pool->IdentifiedComponents.begin(), pool->IdentifiedComponents.end());
  all_comps.insert(all_comps.end(), pool->AnonymousComponents.begin(), pool->AnonymousComponents.end());

  RenderComponentGroup("General Components", &s_GroupGeneralOpen, all_comps, dl, list_w);

  // Other groups here

  ImGui::EndChild();
  ImGui::PopStyleColor();

  {
    ImVec2 wp = ImGui::GetWindowPos();
    dl->AddLine(
        ImVec2(wp.x + list_w, wp.y),
        ImVec2(wp.x + list_w, wp.y + ImGui::GetWindowHeight()),
        ImGui::ColorConvertFloat4ToU32(kSeparator),
        1.0f);
  }

  ImGui::SameLine();

  ImGui::PushStyleColor(ImGuiCol_ChildBg, kBgDark);
  ImGui::BeginChild("##details", ImVec2(0, 0), false);

  if (s_SelectedComponent) {
    RenderComponentDetails(s_SelectedComponent);
  } else {
    ImVec2 avail = ImGui::GetContentRegionAvail();
    ImGui::SetCursorPos(ImVec2(avail.x * 0.5f - 80, avail.y * 0.45f));
    ImGui::PushStyleColor(ImGuiCol_Text, kTextMuted);
    ImGui::TextUnformatted("← Select a component");
    ImGui::PopStyleColor();
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::PopStyleVar(4);
}

#endif  // CHERRY_DEBUG