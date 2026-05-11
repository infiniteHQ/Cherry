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

  {
    auto foo = CherryHook({
      static float hue = 0.0f;

      hue += 0.01f;
      if (hue > 1.0f)
        hue = 0.0f;

      float r = 0.f, g = 0.f, b = 0.f;

      float h = hue * 6.0f;
      int sector = (int)h;
      float f = h - sector;

      float q = 1.0f - f;
      float t = f;

      switch (sector % 6) {
        case 0:
          r = 1.f;
          g = t;
          b = 0.f;
          break;
        case 1:
          r = q;
          g = 1.f;
          b = 0.f;
          break;
        case 2:
          r = 0.f;
          g = 1.f;
          b = t;
          break;
        case 3:
          r = 0.f;
          g = q;
          b = 1.f;
          break;
        case 4:
          r = t;
          g = 0.f;
          b = 1.f;
          break;
        case 5:
          r = 1.f;
          g = 0.f;
          b = q;
          break;
      }

      int R = (int)(r * 255.0f);
      int G = (int)(g * 255.0f);
      int B = (int)(b * 255.0f);

      char hex[8];
      snprintf(hex, sizeof(hex), "#%02X%02X%02X", R, G, B);

      self->SetData("color", std::string(hex));
    });
    Cherry::CreateHook("rgb_channel", CherryHookFrame, foo);
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
  {
    CherryStyle::AddMarginX(10.0f);
    CherryNextComponent.SetProperty("color_text", Cherry::GetHookData(CherryID("rgb_channel"), "color"));
    CherryKit::TextSimple("This is a RGB text !");
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