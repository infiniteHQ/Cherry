#include "../../../cherry.hpp"

/* Component Property level :

Higher level (high priority)
->  -> NextComponent.SetProperty()
-> -> Cherry::SetNextComponentProperty()
-> -> Cherry::PushPermanentProperty()
-> -> [Component]->SetProperty()
Lower level

*/

void DynamicVars() {

  CherryNextComponent.SetIdentifierProperty(ComponentProperty::CreateOnly);
  auto &cmp = CherryKit::ButtonText("Button 1");

  CherryNextComponent.SetProperty("color_bg", "#FF5555");
  CherryNextComponent.SetProperty("color_border", "#FF5555");
  CherryKit::ButtonText("Button 2");

  CherryNextComponent.SetProperty("color_bg", "#55FF55");
  CherryNextComponent.SetProperty("color_border", "#55FF55");
  CherryApp.RenderComponent(cmp);
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

  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}