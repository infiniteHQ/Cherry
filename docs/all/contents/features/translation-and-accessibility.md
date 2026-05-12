Cherry includes a full API for managing translations within your application, allowing you to easily create multilingual software using Cherry's locale system!

You can add your own translation files and define a default one. If other translation files are invalid or incomplete, Cherry will automatically fall back to the default file. This is a great way to give developers time to complete translations without breaking the application.

### Traduction files
We have created a simple translation file format based on JSON.
Here’s what it looks like:

###### en.json

``` json
{
    "locales": [ 
        {"loc.menubar.menu.vortex":"Vortex"},
        {"loc.menubar.menu.options":"Options"},
        {"loc.menubar.summary.general":"General"},
        {"loc.menubar.summary.vortex_labs":"Vortex Labs"},
        {"loc.menubar.summary.manage":"Manage"},
        {"loc.menubar.summary.other":"Other"},
        {"loc.menubar.summary.utilities":"Utilities"},
        {"loc.menubar.summary.settings":"Settings"},
    ]
}
```

Then, the translation will be loaded into Cherry whenever a component requests a translation.

###### example
```cpp
// In this example, the text displayed on this button will be "Utilities".
CherryKit::ButtonText(Cherry::GetLocale("loc.menubar.summary.utilities"));
```

### Add traduction file
Afin d'ajouter un fichier de traductionm il suffit de l'inclure au moment de la creation de votre application.

```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  // Other configs...
  // NOTE: We will be able to do this to the config soon
  Cherry::Application *app = new Cherry::Application(config);

  app->AddLocale("fr", "path/to/locales.json");
  app->AddLocale("en", Cherry::GetPath("resources/locales/en.json"));
  app->SetDefaultLocale("en");
  app->SetLocale("en");

  // Other configs...
  return app;
}
```

In this example, we see how to add a translation file using `app->AddLocale("fr", "path/to/locales.json");`. We also need to define the name of the translation before including the file, which will be the way to identify this translation file.

```cpp
app->AddLocale("fr", "path/to/locales.json");
```

You can use `GetPath` via a resource folder to import the file as an asset.  
<banner type="note"> To learn how to add and use assets, refer to the section [Paths & Assets](https://i.n:content_name=learn&page_name=path_assets&section=all) </banner>


```cpp  
app->AddLocale("en", Cherry::GetPath("resources/locales/en.json"));
```

We then see that we can define the translation file to use by default. Cherry will always look into the default file if it cannot find a translation in another selected file.

```cpp
app->AddLocale("en", Cherry::GetPath("resources/locales/en.json"));
app->AddLocale("fr", Cherry::GetPath("resources/locales/fr.json"));
app->SetDefaultLocale("en");
app->SetLocale("fr");

// If a translation does not exist on "fr", Cherry will search on the default : "en". 
```

### Set traduction

You can now select the translation file to use at any time via `CherryApp.SetLocale("translation")`.

```cpp
  if (CherryKit::ButtonText("Turn to English").GetDataAs<bool>("isClicked")) {
    CherryApp.SetLocale("en");
  }

  if (CherryKit::ButtonText("Turn to French").GetDataAs<bool>("isClicked")) {
    CherryApp.SetLocale("fr");
  }
```