#include "../../cherry.hpp"

void LoadHook() {
  {
    auto foo = CherryHook({
      static int i = 0;
      i++;
      self->SetData("seconds", std::to_string(i));
    });

    auto cond = CherryHookCond({
      if (CherryApp.GetComponent(CherryID("btn")).GetDataAs<bool>("isPressed")) {
        return true;
      }
      return false;
    });
    Cherry::CreateHook("btn_time_pressed", CherryHookSecond, foo, cond);
  }

  {
    auto foo = CherryHook({
      static int i = 0;
      i++;
      self->SetData("clicked", std::to_string(i));
    });

    auto cond = CherryHookCond({
      if (CherryApp.GetComponent(CherryID("btn")).GetDataAs<bool>("isClicked")) {
        return true;
      }
      return false;
    });
    Cherry::CreateHook("btn_number_of_clicks", CherryHookFrame, foo, cond);
  }
}

void Render() {
  LoadHook();
  CherryKit::ButtonText(CherryID("btn"), "button");

  {
    std::string val = Cherry::GetHookData(CherryID("btn_time_pressed"), "seconds");
    CherryKit::TextSimple("Seconds pressed :");
    CherryStyle::AddMarginX(10.0f);
    CherryKit::TextSimple(val);
  }
  {
    std::string val = Cherry::GetHookData(CherryID("btn_number_of_clicks"), "clicked");
    CherryKit::TextSimple("Number of clicks :");
    CherryStyle::AddMarginX(10.0f);
    CherryKit::TextSimple(val);
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