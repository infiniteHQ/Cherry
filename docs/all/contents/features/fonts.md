Cherry allows you to manage fonts. You can add and define fonts in your application. You can also vary fonts within the same window or even within a single component.

<banner type="note">Cherry support TTF and OTF files</banner> 

<picture>
  <img width=380 src="https://static.infinite.si/cherrydocs/1.6/all/media/fonts.png">
</picture>

```cpp
void Render() {
  Cherry::PushFont("rocket");
  CherryKit::TitleOne("Hello font !!!");
  Cherry::PopFont();

  Cherry::PushFont("chunky");
  CherryKit::TitleOne("Hello font !!!");
  Cherry::PopFont();

  Cherry::PushFont("jetbrainsmono");
  CherryKit::TitleOne("Hello font !!!");
  Cherry::PopFont();
}

CherryApplication CherryMain(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  auto app = new CherryApplication(config);
  
  // Add OTF files
  app->AddFont("rocket", CherryPath("rocket.otf"), 80.0f);
  app->AddFont("chunky", CherryPath("chunky.otf"), 80.0f);
  // Add TTF files
  app->AddFont("jetbrainsmono", CherryPath("jetbrainsmono.ttf"), 80.0f);

  return app;
}
```

### Add fonts
You can directly add fonts to the application during its creation, so that the font will be available throughout the entire context.

```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  // Other configs...
  // Set the name, the file and default size of the font.
  // NOTE: We will be able to do this to the config soon
  Cherry::Application *app = new Cherry::Application(config);
  app->AddFont("Consola", "path/to/font.ttf", 17.0f);
  // Other configs...
  return app;
}
```

You can also import a font with a ressource folder :
```cpp
app->AddFont("Consola", Cherry::GetPath("resources/fonts/consola.ttf"), 17.0f);
```

<banner type="note">To learn how to add and use assets, refer to the section [Paths & Assets](https://i.n:content_name=learn&page_name=path_assets&section=all) </banner>


Once added, the font will be available throughout the entire context by its name — in our example, `"Consola"`.

### Use fonts

As you can see in the following example, we directly use the API through `Cherry::PushFont` and `Cherry::PopFont` to define which font to use and when.

```cpp
Cherry::PushFont("Consola");
CherryKit::TextSimple("Text with font !");
Cherry::PopFont();
```

