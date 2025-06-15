#include "../../../cherry.hpp"

void RenderModes() {
  // This exemple show the capabilities of Render Mode.
  //
  // Brief :
  // - RenderMode::CreateOnly : Create without render (static component)
  // - RenderMode::Inline : Render without create (immediate mode component)
  // - RenderMode::None : Render and create (semi/static component)

  // As you can see, we declare button 1, 2 and 3. But we render 3, 2 and 1.

  // We can se CreateOnly to the CherryNextComponent object
  CherryNextComponent.SetRenderMode(RenderMode::CreateOnly);
  auto &cmp = CherryKit::ButtonText("Button 1");

  // We can se CreateOnly by pushing to context the RenderMode
  CherryApp.PushComponentRenderMode(RenderMode::CreateOnly);
  auto &cmp2 = CherryKit::ButtonText("Button 2");
  CherryApp.PopRenderMode();

  // SemiStatic rendering, default RenderMode::None
  CherryNextComponent.SetProperty("color_border", "#55FF55");
  CherryKit::ButtonText("Button 3");

  // Called rendering
  CherryNextComponent.SetProperty("color_border", "#FF5555");
  CherryApp.RenderComponent(cmp2);

  // Called rendering
  CherryNextComponent.SetProperty("color_border", "#5555FF");
  CherryApp.RenderComponent(cmp);
}

void DynamicVars() {
  // This exemple show objects and components references you can call and
  // manipulate, also called "Dynamic Variables"

  CherryKit::SeparatorText("CherryLastComponent / CherryNextComponent");

  // We declare a simple button with green borders and some data with
  // CherryNextComponent.
  CherryNextComponent.SetProperty("color_border", "#55FF55");
  CherryNextComponent.SetData("test", 42);
  CherryKit::ButtonText("Super button");
  {
    // We can call the last rendered component reference and manipulate or
    // control it with CherryLastComponent.
    auto last_component = CherryLastComponent;
    CherryKit::TextSimple("CherryLastComponent datas :");
    CherryKit::TextSimple("ID : " + last_component.GetIdentifier().string());
    CherryKit::TextSimple("\"test\" data : " + last_component.GetData("test"));
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
    CherryKit::TextSimple("CherryLastComponent datas :");
    CherryKit::TextSimple("ID : " + last_component.GetIdentifier().string());
    CherryKit::TextSimple("Group : " +
                          last_component.GetIdentifier().component_group());
    CherryKit::TextSimple("\"test\" data : " + last_component.GetData("test"));
  }

  CherryApp.AddDataToComponentGroup("test", "test", "OK");
}

void ComponentPools() {
  // This is a example of a custom component pool, different than the main app
  // context.
  static ComponentsPool custom_pool;

  // We push the custom pool as default pool to create component in the main
  // application.
  CherryApp.PushComponentPool(&custom_pool);

  // We can so, create a anonymous component in this custom pool.
  CherryKit::ButtonText("Anonymous button in custom pool");

  // After all, we remove the current custom pool.
  CherryApp.PopComponentPool();

  CherryKit::ButtonText(CherryID(&custom_pool, "my_button"),
                        "Other button in custom pool by ID");

  {
    CherryKit::TextSimple(
        "Custom pool components size : " +
        std::to_string(custom_pool.IdentifiedComponents.size()));
  }
}

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

  CherryKit::TextSimple(
      CherryID("Mooving0"),
      "Custom pool anonymous components pool size : " +
          std::to_string(custom_pool.AnonymousComponents.size()));
  CherryKit::TextSimple(
      CherryID("Mooving1"),
      "Custom pool identified components pool size : " +
          std::to_string(custom_pool.IdentifiedComponents.size()));
  CherryKit::TextSimple(
      CherryID("Mooving2"),
      "Runtime anonymous components pool size : " +
          std::to_string(
              CherryApp.m_ApplicationComponentPool.AnonymousComponents.size()));
  CherryKit::TextSimple(CherryID("Mooving3"),
                        "Runtime identified components pool size : " +
                            std::to_string(CherryApp.m_ApplicationComponentPool
                                               .IdentifiedComponents.size()));
}

CherryApplication CherryMain(int argc, char **argv) {
  CherryAppConfig config;

  config.SetRenderMode(Cherry::WindowRenderingMethod::TabWidows);

  // Cherry::AddAppWindow(CherryKit::WindowSimple("Base", Base));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("Dynamic Variables", DynamicVars));
  Cherry::AddAppWindow(CherryKit::WindowSimple("Render Modes", RenderModes));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("Component Pools", ComponentPools));
  Cherry::AddAppWindow(
      CherryKit::WindowSimple("Component Groups", ComponentGroups));

  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}