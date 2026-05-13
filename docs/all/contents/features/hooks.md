

Cherry gives you the possibility of creating hooks in your code. A hook is an independant process that will be calculated during frames or every second. Hooks can process animation data, compute shader results, etc.

> The following example will calculate an active RGB color, and display the color in the text.
```cpp
// Before render
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

    switch (sector % 6)
    {
        case 0: r = 1.f; g = t;   b = 0.f; break;
        case 1: r = q;   g = 1.f; b = 0.f; break;
        case 2: r = 0.f; g = 1.f; b = t;   break;
        case 3: r = 0.f; g = q;   b = 1.f; break;
        case 4: r = t;   g = 0.f; b = 1.f; break;
        case 5: r = 1.f; g = 0.f; b = q;   break;
    }

    int R = (int)(r * 255.0f);
    int G = (int)(g * 255.0f);
    int B = (int)(b * 255.0f);

    char hex[8];
    snprintf(hex, sizeof(hex), "#%02X%02X%02X", R, G, B);

    self->SetData("color", hex);
});

Cherry::CreateHook("rgb_channel", CherryHookSecond, foo);
 
// Render
void Render() {
std::string color = Cherry::GetHookData(CherryID("rgb_channel"), "color");
}
```
There is the result :

<img width="800" height="450" alt="hooks" src="https://static.infinite.si/cherrydocs/1.6/all/media/hooks.gif" />

> The following example will create a hook that counts the number of clicks on the button "btn".
```cpp
// Before render
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
 
// Render
void Render() {
CherryKit::ButtonText(CherryID("btn"), "button");

std::string val = Cherry::GetHookData(CherryID("btn_number_of_clicks"), "clicked");
}
```
