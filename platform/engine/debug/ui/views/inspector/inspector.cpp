
#include "inspector.hpp"
#ifdef CHERRY_DEBUG
#include "../../../../app/app.hpp"

static std::shared_ptr<Cherry::Component> s_SelectedComponent = nullptr;

InspectorView::InspectorView(const std::string &name) {
  m_AppWindow = std::make_shared<Cherry::AppWindow>(name, name);
  m_AppWindow->SetInternalPaddingX(0.0f);
  m_AppWindow->SetInternalPaddingY(0.0f);
  std::shared_ptr<Cherry::AppWindow> win = m_AppWindow;
}

std::shared_ptr<Cherry::AppWindow> &InspectorView::GetAppWindow() {
  return m_AppWindow;
}

std::shared_ptr<InspectorView> InspectorView::Create(const std::string &name) {
  auto instance = std::shared_ptr<InspectorView>(new InspectorView(name));
  instance->SetupRenderCallback();
  return instance;
}

void InspectorView::SetupRenderCallback() {
  auto self = shared_from_this();
  m_AppWindow->SetRenderCallback([self]() {
    if (self) {
      self->Render();
    }
  });
}

void InspectorView::RenderSimpleTable(
    const char *label, const std::unordered_map<std::string, std::string> &map,
    ImVec4 key_color) {
  if (map.empty()) {
    if (ImGui::CollapsingHeader(label)) {
      ImGui::Indent();
      ImGui::TextDisabled("Empty");
      ImGui::Unindent();
    }
    return;
  }

  if (ImGui::CollapsingHeader(label, ImGuiTreeNodeFlags_DefaultOpen)) {
    if (ImGui::BeginTable(label, 2,
                          ImGuiTableFlags_BordersInnerH |
                              ImGuiTableFlags_RowBg |
                              ImGuiTableFlags_Resizable)) {
      ImGui::TableSetupColumn("Key", ImGuiTableColumnFlags_WidthFixed, 130.0f);
      ImGui::TableSetupColumn("Value");

      for (auto const &[key, val] : map) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::TextColored(key_color, "%s", key.c_str());

        ImGui::TableSetColumnIndex(1);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.85f, 0.85f, 0.85f, 1.0f));
        ImGui::TextUnformatted(val.c_str());
        ImGui::PopStyleColor();
      }
      ImGui::EndTable();
    }
  }
}

void InspectorView::RenderComponentDetails(
    std::shared_ptr<Cherry::Component> &comp) {
  if (!comp)
    return;

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.11f, 0.11f, 1.0f));
  ImGui::BeginChild("DetailsHeader", ImVec2(0, 38), false);

  ImGui::SetCursorPos(ImVec2(10, 10));
  ImGui::TextDisabled("node");
  ImGui::SameLine();
  ImGui::Text(">");
  ImGui::SameLine();

  ImGui::TextColored(ImVec4(0.9f, 0.8f, 0.4f, 1.0f), "%s",
                     comp->GetType().c_str());

  std::string id_str = comp->GetIdentifier().string();
  if (!id_str.empty()) {
    ImGui::SameLine();
    ImGui::TextDisabled("#");
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.4f, 0.7f, 1.0f, 1.0f), "%s", id_str.c_str());
  }

  ImGui::SameLine(ImGui::GetWindowWidth() - 85);
  if (comp->m_IsComponentRendered) {
    ImGui::TextColored(ImVec4(0.3f, 0.8f, 0.3f, 1.0f), "● Rendered");
  } else {
    ImGui::TextDisabled("○ Hidden");
  }

  ImGui::EndChild();
  ImGui::PopStyleColor();

  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));

  if (ImGui::BeginTabBar("InspectorTabs", ImGuiTabBarFlags_None)) {

    if (ImGui::BeginTabItem("Properties")) {
      RenderSimpleTable("Internal Properties", comp->GetPropertiesMap(),
                        ImVec4(0.5f, 0.75f, 1.0f, 1.0f));
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Data")) {
      RenderSimpleTable("Component Data", comp->GetDataMap(),
                        ImVec4(0.4f, 1.0f, 0.4f, 1.0f));
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("Context")) {
      RenderSimpleTable("Context Properties", comp->GetContextPropertiesMap(),
                        ImVec4(0.4f, 0.6f, 0.9f, 1.0f));
      RenderSimpleTable("Context Data", comp->GetContextDataMap(),
                        ImVec4(1.0f, 0.4f, 1.0f, 1.0f));
      ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("API Support")) {
      ImGui::Spacing();
      if (ImGui::CollapsingHeader("Interface Methods",
                                  ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent();
        ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "f");
        ImGui::SameLine();
        ImGui::Text("OnUpdate(delta_time)");

        ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "f");
        ImGui::SameLine();
        ImGui::Text("OnRender()");

        ImGui::TextColored(ImVec4(0.8f, 0.5f, 1.0f, 1.0f), "f");
        ImGui::SameLine();
        ImGui::Text("RebuildProperties()");
        ImGui::Unindent();
      }

      if (ImGui::CollapsingHeader("Scripting Support")) {
        ImGui::Indent();
        ImGui::TextDisabled("Lua Binding: ");
        ImGui::SameLine();
        ImGui::Text("cherry.component.%s", comp->GetType().c_str());
        ImGui::Unindent();
      }
      ImGui::EndTabItem();
    }

    ImGui::EndTabBar();
  }

  ImGui::PopStyleVar();
}

void InspectorView::Render() {
  auto pool = CherryApp.GetComponentPool();
  if (!pool)
    return;

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

  static int active_main_tab = 0;
  const char *main_tabs[] = {"Components", "Effects", "Animations", "Drawing",
                             "Scripting"};

  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
  ImGui::BeginChild("TopNav", ImVec2(0, 34), false,
                    ImGuiWindowFlags_NoScrollbar);

  ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 8));
  float total_width = ImGui::GetContentRegionAvail().x;
  float tab_width = total_width / 5.0f;

  for (int i = 0; i < 5; i++) {
    bool is_active = (active_main_tab == i);

    if (is_active) {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.18f, 0.18f, 0.18f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.8f, 1.0f, 1.0f));
    } else {
      ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.08f, 0.08f, 0.08f, 1.0f));
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
    }

    if (ImGui::Button(main_tabs[i], ImVec2(tab_width, 34))) {
      active_main_tab = i;
    }

    ImGui::PopStyleColor(2);
    if (i < 4)
      ImGui::SameLine();
  }
  ImGui::PopStyleVar();
  ImGui::EndChild();
  ImGui::PopStyleColor();
  ImGui::Separator();
  if (active_main_tab == 0) {

    float left_panel_w = ImGui::GetContentRegionAvail().x * 0.35f;

    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.11f, 0.11f, 0.11f, 1.0f));
    ImGui::BeginChild("LeftTreePanel", ImVec2(left_panel_w, 0), true);

    ImGui::Spacing();
    if (ImGui::TreeNodeEx("Application Pools",
                          ImGuiTreeNodeFlags_DefaultOpen |
                              ImGuiTreeNodeFlags_SpanAvailWidth)) {

      std::vector<std::shared_ptr<Cherry::Component>> all_comps;
      all_comps.insert(all_comps.end(), pool->IdentifiedComponents.begin(),
                       pool->IdentifiedComponents.end());
      all_comps.insert(all_comps.end(), pool->AnonymousComponents.begin(),
                       pool->AnonymousComponents.end());

      for (auto &comp : all_comps) {
        if (!comp)
          continue;

        bool is_selected = (s_SelectedComponent == comp);
        std::string label = comp->GetIdentifier().string();
        if (label.empty())
          label = "anonymous-node";

        ImGui::PushID(comp.get());
        ImGui::BeginGroup();

        ImVec2 p = ImGui::GetCursorScreenPos();
        if (is_selected)
          draw_list->AddRectFilled(
              p, ImVec2(p.x + 3, p.y + ImGui::GetTextLineHeightWithSpacing()),
              IM_COL32(100, 200, 255, 255));

        ImGui::Indent(2);
        if (ImGui::Selectable(label.c_str(), is_selected,
                              ImGuiTreeNodeFlags_SpanAvailWidth)) {
          s_SelectedComponent = comp;
        }

        ImGui::Unindent(2);

        ImGui::EndGroup();
        ImGui::PopID();
      }
      ImGui::TreePop();
    }

    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::SameLine();

    ImGui::BeginChild("RightDetailsPanel", ImVec2(0, 0), true);
    if (s_SelectedComponent) {
      RenderComponentDetails(s_SelectedComponent);
    } else {
      ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x * 0.5f - 100,
                                 ImGui::GetContentRegionAvail().y * 0.45f));
      ImGui::TextDisabled("Select a component to inspect");
    }
    ImGui::EndChild();

  } else {
    ImGui::BeginChild("WIPContent", ImVec2(0, 0), true,
                      ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(ImVec2(ImGui::GetContentRegionAvail().x * 0.4f,
                               ImGui::GetContentRegionAvail().y * 0.4f));
    ImGui::TextColored(ImVec4(1, 0.6f, 0, 1), "Work In Progress");
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x * 0.4f);
    ImGui::TextDisabled("Come back at v1.6!");
    ImGui::EndChild();
  }

  ImGui::PopStyleVar(3);
}

#endif // CHERRY_DEBUG