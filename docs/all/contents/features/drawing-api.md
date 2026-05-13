Cherry has a high-level and simple-to-use drawing API, available in Lua and C++. With it, you can draw shapes and primitive forms, and bring life to your interfaces.

> Simple example using C++ drawing API
```cpp
void Render() {
  float x = 50.0f;
  float y = 75.0f;
  float w = 90.0f;
  float h = 20.0f;
  std::string color = "#B1FF31";
  CherryDrawing::RectWindow(Cherry::Vec2(x, y), Cherry::Vec2(w, h), color);
}
```
Result :

<picture>
  <img width=380 src="https://static.infinite.si/cherrydocs/1.6/all/media/drawingcpp.png">
</picture>

> More advanced example using C++ drawing API

<img width="800" height="450" alt="gif" src="https://static.infinite.si/cherrydocs/1.6/all/media/drawing.gif" />

> This is the [examples/concepts/drawing](https://github.com/infiniteHQ/Cherry/tree/main/examples/concepts/drawing) example of this repository ! You can check the code [here](https://github.com/infiniteHQ/Cherry/tree/main/examples/concepts/drawing)
