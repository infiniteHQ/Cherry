Cherry embeds a high-level and secure image and texture loader, compatible with multiple contexts of Dear ImGui.

`Example using images in components :`
```cpp  
CherryStyle::AddMarginX(50.0f);
CherryStyle::AddMarginY(50.0f);
CherryKit::ImageLocal(CherryPath("resources/images/infinite.png"), 100.0f, 50.0f);

CherryKit::ButtonText("Click me !");
CherryKit::ButtonText("And me !");
CherryKit::ButtonTextImage("Infinityyy", CherryPath("resources/images/settings.png"));
```

<picture>
  <img width=380 src="https://static.infinite.si/cherrydocs/1.6/all/media/image.png">
</picture>

You can also use Cherry to get native Dear ImGui textures :
`This is how :`
```cpp
ImTextureID t = Cherry::GetTexture(CherryPath("resources/images/infinite.png"));
```