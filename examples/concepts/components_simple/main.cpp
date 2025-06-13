#include "../../../cherry.hpp"

void DynamicVars() {
  // We can safely make mistake by call invalid latest components
  {
    auto last_component = CherryLastComponent;
    CherryKit::TextSimple("Invalid component :");
    CherryKit::TextSimple("ID : " + last_component.GetIdentifier().string());
    CherryKit::TextSimple("isClicked : " + last_component.GetData("isClicked"));
    CherryKit::TextSimple("isPressed : " + last_component.GetData("isPressed"));
    CherryKit::TextSimple("isHovered : " + last_component.GetData("isHovered"));
  }

  CherryKit::Separator();

  // We can also get the last anonymous component
  CherryKit::ButtonText("Button");

  {
    auto last_component = CherryLastComponent;
    CherryKit::TextSimple("Valid anonymous button :");
    CherryKit::TextSimple("ID : " + last_component.GetIdentifier().string());
    CherryKit::TextSimple("isClicked : " + last_component.GetData("isClicked"));
    CherryKit::TextSimple("isPressed : " + last_component.GetData("isPressed"));
    CherryKit::TextSimple("isHovered : " + last_component.GetData("isHovered"));
  }

  CherryKit::Separator();

  // We can get the last identified compoent
  CherryKit::ButtonText(CherryID("my_id"), "Button with ID");

  {
    auto last_component = CherryLastComponent;
    CherryKit::TextSimple("Valid button with ID :");
    CherryKit::TextSimple("ID : " + last_component.GetIdentifier().string());
    CherryKit::TextSimple("isClicked : " + last_component.GetData("isClicked"));
    CherryKit::TextSimple("isPressed : " + last_component.GetData("isPressed"));
    CherryKit::TextSimple("isHovered : " + last_component.GetData("isHovered"));
  }
}

void ComponentGroups() {
  CherryApp.PushComponentGroup("test");
  CherryKit::ButtonText("test");
  CherryApp.PopComponentGroup();
  {
    auto last_component = CherryLastComponent;
    CherryKit::TextSimple("Valid button with ID :");
    CherryKit::TextSimple("ID : " + last_component.GetIdentifier().string());
    CherryKit::TextSimple("test : " + last_component.GetData("test"));
  }

  CherryApp.AddDataToComponentGroup("test", "test", "OK");
}

void LogImGuiContextState() {
  ImGuiContext &g = *ImGui::GetCurrentContext();
  ImGuiStyle &style = ImGui::GetStyle();

  std::cout << "=== ImGui Context Diagnostics ===" << std::endl;

  // Style stack sizes
  std::cout << "[STACK] StyleColor Stack Size:   " << g.ColorStack.Size
            << std::endl;
  std::cout << "[STACK] Font Stack Size:         " << g.FontStack.Size
            << std::endl;
  std::cout << "[STACK] StyleVar Stack Size:     " << g.StyleVarStack.Size
            << std::endl;
  // std::cout << "[STACK] ID Stack Size:           " << g.IDStack.Size <<
  // std::endl;
  std::cout << "[STACK] Group Stack Size:        " << g.GroupStack.Size
            << std::endl;
  std::cout << "[STACK] Disabled Stack Size:     " << g.DisabledStackSize
            << std::endl;

  // Windows
  std::cout << "[WINDOWS] Active Windows:        " << g.Windows.Size
            << std::endl;

  // Draw data
  ImDrawData *drawData = ImGui::GetDrawData();
  if (drawData) {
    std::cout << "[DRAW] Total Draw Lists:         " << drawData->CmdListsCount
              << std::endl;
    std::cout << "[DRAW] Total Vtx Count:          " << drawData->TotalVtxCount
              << std::endl;
    std::cout << "[DRAW] Total Idx Count:          " << drawData->TotalIdxCount
              << std::endl;
  }

  // Font atlas memory (if leaked, may grow)
  ImFontAtlas *atlas = ImGui::GetIO().Fonts;
  if (atlas) {
    std::cout << "[FONT] Font Atlas TexID:         " << (void *)atlas->TexID
              << std::endl;
    std::cout << "[FONT] Font Count:               " << atlas->Fonts.Size
              << std::endl;
    std::cout << "[FONT] Tex Width/Height:         " << atlas->TexWidth << "x"
              << atlas->TexHeight << std::endl;
  }

  std::cout << "=================================\n" << std::endl;
}

void Baset() { LogImGuiContextState(); }
void Base() {
  ///////////////////////////////////////////////////////////////////////////////////////////////
  CherryKit::SeparatorText("Anonymous Component in public runtime");
  // This component is anonymous (no CherryID specified), and is public (no
  // custom component pool specified in the runtime or in the CherryID) The only
  // way we can remotly interact with it is with Dynamic variables
  // (CherryLastComponent/CherryNextComponent), or with storing the returned
  // shared_ptr of this component
  CherryKit::ButtonText("Button without ID");
  ///////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////////////////////
  CherryKit::SeparatorText("Identified Component in public runtime");
  // This component is identified, and is public (no custom component pool
  // specified in the runtime or in the CherryID) We can now identify and
  // remotly interact with this component with it's CherryID");
  CherryKit::ButtonText(CherryID("button_id"), "Button with ID");
  ///////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////////////////////
  CherryKit::SeparatorText("Anonymous Component in private pool");

  static ComponentsPool custom_pool;
  // We push the custom pool as default pool to create component in the main
  // application.
  CherryApp.PushComponentPool(&custom_pool);

  // We can so, create a anonymous component in this custom pool.
  CherryKit::ButtonText("Anonymous button in custom pool");

  // After all, we remove the current custom pool.
  CherryApp.PopComponentPool();

  ///////////////////////////////////////////////////////////////////////////////////////////////

  ///////////////////////////////////////////////////////////////////////////////////////////////
  CherryKit::SeparatorText("Identified Component in private pool");
  // We can use Push/PopComponentArray, but we can also specify the custom
  // component pool in the CherryID directly.
  // CherryKit::ButtonText(CherryID(&custom_pool, "button_id"), "Button with ID
  // in custom pool");
  ///////////////////////////////////////////////////////////////////////////////////////////////
}

CherryApplication CherryMain(int argc, char **argv) {
  CherryAppConfig config;

  config.SetRenderMode(Cherry::WindowRenderingMethod::TabWidows);

  Cherry::AddAppWindow(CherryKit::WindowSimple("Base", Baset));

  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}