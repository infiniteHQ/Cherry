#include "../../cherry.hpp"

void Render() {
  if (CherryApp.IsKeyPressed(Cherry::CherryKey::Z)) {
    CherryNextComponent.SetProperty("color_text", "#12AA12");
    CherryKit::TextSimple("Z Pressed !");
  } else {
    CherryNextComponent.SetProperty("color_text", "#AA1212");
    CherryKit::TextSimple("Z not pressed !");
  }
  if (CherryApp.IsKeyPressed(Cherry::CherryKey::LEFT)) {
    CherryNextComponent.SetProperty("color_text", "#12AA12");
    CherryKit::TextSimple("< Pressed !");
  } else {
    CherryNextComponent.SetProperty("color_text", "#AA1212");
    CherryKit::TextSimple("< not pressed !");
  }
  if (CherryApp.IsMouseClicked(0)) {
    CherryNextComponent.SetProperty("color_text", "#12AA12");
    CherryKit::TextSimple("Left Click Clicked !");
  } else {
    CherryNextComponent.SetProperty("color_text", "#AA1212");
    CherryKit::TextSimple("Left Click not Clicked !");
  }
  if (CherryApp.IsMousePressed(0)) {
    CherryNextComponent.SetProperty("color_text", "#12AA12");
    CherryKit::TextSimple("Left Click Pressed !");
  } else {
    CherryNextComponent.SetProperty("color_text", "#AA1212");
    CherryKit::TextSimple("Left Click not pressed !");
  }
}

CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.SetMainRenderCallback(Render);
  return new CherryApplication(config);
}

int main(int argc, char *argv[]) {
  CherryRun(argc, argv);
  return 0;
}