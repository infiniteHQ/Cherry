Cherry provides 8 themes of your choice, but [you can create your own](#themes-builder).

To access the built-in themes, simply enable `CHERRY_ENABLE_KIT` in your CMake file.

Then, you have access to 8 basic themes.

```cpp
// You can call built-in themes with CherryThemes::

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;

  config.AddTheme(CherryThemes::Dark());
  config.AddTheme(CherryThemes::Light());
  config.AddTheme(CherryThemes::DarkColorfull());

  // Other settings like render mode, windows etc...

  return new CherryApplication(config);
}
```