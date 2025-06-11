#include "../../../cherry.hpp"

void Render() {

  std::cout << "G" << std::endl;

  CherryKit::TextSimple(CherryLastComponent.GetIdentifier().string());

  CherryKit::SeparatorText("Without ID");
  // Cherry::NextComponent()->SetData() (remplace SetNextComponentProperty)
  // Cherry::NextComponent()->SetProperty()
  // better: Cherry::NextComponent().SetProperty() (ref)
  // == CherryNextComponent.SetProperty() (like other contant variables)
  // == CherryPreviousComponent.SetProperty() (like other contant variables)
  // == CherryCurrentTheme.SetProperty() (like other contant variables)

  if (CherryKit::ButtonText("Button without ID")->GetData("isClicked") ==
      "true") {
  }
  CherryKit::TextSimple("Component CherryID"
                        "my_id");
  CherryKit::TextSimple("Component isClicked"
                        "true");
  CherryKit::TextSimple("Component isHovered"
                        "true");
  CherryKit::TextSimple("Component isPressed"
                        "true");
  CherryKit::TextSimple("Component isActive"
                        "true");

  // Cherry::GetLastComponent()->GetData()

  CherryKit::SeparatorText("With ID");

  if (CherryKit::ButtonText(CherryID("my_id"), "Button with ID")
          ->GetData("isClicked") == "true") {
  }

  CherryKit::TextSimple("Component CherryID"
                        "my_id");
  CherryKit::TextSimple("Component isClicked"
                        "true");
  CherryKit::TextSimple("Component isHovered"
                        "true");
  CherryKit::TextSimple("Component isPressed"
                        "true");
  CherryKit::TextSimple("Component isActive"
                        "true");
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.SetRenderMode(Cherry::WindowRenderingMethod::TabWidows);

  Cherry::AddAppWindow(CherryKit::WindowSimple("Components", Render));
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  std::thread([&]() { Cherry::Main(argc, argv); }).join();
  return 0;
}