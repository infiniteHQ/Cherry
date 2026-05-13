
#### HTTP fetches

There are built-in network features to fetch files from URLs, primarily for the purpose of downloading images from the web.

```cpp
void Render() {
  // Net components from the Cherry Kit
  CherryKit::ImageHttp("https://infinite.si/assets/in_projects.c64028e7.png");

  // Or fetch as Dear ImGui texture directly
  auto texture = Cherry::GetTexture(Cherry::GetHttpPath("https://infinite.si/assets/in_projects.c64028e7.png"));
  CherryGUI::Image(texture, 240.0f, 150.0f);
}
```

Result :

<picture>
  <img width=380 src="https://static.infinite.si/cherrydocs/1.6/all/media/netfetch.png">
</picture>

> Cherry will automaticly put images, medias or http files into a cache in temporary folder to avoid making too many fetches and preserve resources.