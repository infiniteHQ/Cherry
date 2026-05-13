<bloc>

#### LineWindow
Draws a line between two points using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::LineWindow(Vec2 start, Vec2 end, const std::string& hexcol, float thickness = 1.0f);
```

* **start, end**: Start and end points of the line, relative to the current window position.
* **hexcol**: Color in hexadecimal format (e.g., `"#FF0000FF"`).
* **thickness**: *(optional)* Line thickness in pixels. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::LineWindow({10, 10}, {200, 10}, "#FF0000FF");
CherryDrawing::LineWindow({10, 10}, {200, 10}, "#FF0000FF", 2.0f);
```
</bloc>

<bloc>

#### LineScreen
Draws a line between two points using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::LineScreen(Vec2 start, Vec2 end, const std::string& hexcol, float thickness = 1.0f);
```

* **start, end**: Absolute screen positions.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness in pixels. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::LineScreen({0, 0}, {1920, 1080}, "#FFFFFFFF", 1.5f);
```
</bloc>

<bloc>

#### LineHWindow
Draws a horizontal line at a given Y position, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::LineHWindow(float y, float x_start, float x_end, const std::string& hexcol, float thickness = 1.0f);
```

* **y**: Vertical position of the line (relative to window).
* **x_start, x_end**: Horizontal start and end positions.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::LineHWindow(50, 0, 300, "#CCCCCCFF");
```
</bloc>

<bloc>

#### LineVWindow
Draws a vertical line at a given X position, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::LineVWindow(float x, float y_start, float y_end, const std::string& hexcol, float thickness = 1.0f);
```

* **x**: Horizontal position of the line (relative to window).
* **y_start, y_end**: Vertical start and end positions.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::LineVWindow(100, 0, 400, "#CCCCCCFF");
```
</bloc>

<bloc>

#### LineHScreen
Draws a horizontal line at a given Y position using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::LineHScreen(float y, float x_start, float x_end, const std::string& hexcol, float thickness = 1.0f);
```

* **y**: Absolute vertical position.
* **x_start, x_end**: Absolute horizontal start and end positions.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::LineHScreen(30, 0, 1920, "#000000FF", 2.0f);
```
</bloc>

<bloc>

#### LineVScreen
Draws a vertical line at a given X position using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::LineVScreen(float x, float y_start, float y_end, const std::string& hexcol, float thickness = 1.0f);
```

* **x**: Absolute horizontal position.
* **y_start, y_end**: Absolute vertical start and end positions.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::LineVScreen(960, 0, 1080, "#FFFFFFFF");
```
</bloc>

<bloc>

#### PolylineWindow
Draws a series of connected line segments through a list of points, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::PolylineWindow(const std::vector<Vec2>& points, const std::string& hexcol, float thickness = 1.0f);
```

* **points**: List of at least 2 points defining the polyline path.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::PolylineWindow({{0,0},{50,100},{100,0},{150,100}}, "#00FF00FF", 2.0f);
```
</bloc>

<bloc>

#### PolylineScreen
Draws a series of connected line segments through a list of points using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::PolylineScreen(const std::vector<Vec2>& points, const std::string& hexcol, float thickness = 1.0f);
```

* **points**: List of at least 2 absolute screen points.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::PolylineScreen({{100,100},{200,200},{300,100}}, "#FF00FFFF");
```
</bloc>

<bloc>

#### CrossWindow
Draws a cross (plus sign) centered on a point, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::CrossWindow(Vec2 center, float size, const std::string& hexcol, float thickness = 1.0f);
```

* **center**: Center point of the cross (relative to window).
* **size**: Total length of each arm (the cross spans `size` in both directions).
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::CrossWindow({100, 100}, 20.0f, "#FF0000FF");
```
</bloc>

<bloc>

#### CrossScreen
Draws a cross (plus sign) centered on a point using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::CrossScreen(Vec2 center, float size, const std::string& hexcol, float thickness = 1.0f);
```

* **center**: Absolute screen position of the cross center.
* **size**: Total length of each arm.
* **hexcol**: Color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::CrossScreen({960, 540}, 30.0f, "#FFFFFFFF", 2.0f);
```
</bloc>

<bloc>

#### RectWindow
Draws a filled rectangle using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectWindow(Vec2 pos, Vec2 size, const std::string& hexcol);
```

* **pos**: Top-left corner of the rectangle (relative to window).
* **size**: Width and height of the rectangle.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::RectWindow({10, 10}, {200, 100}, "#3399FFFF");
```
</bloc>

<bloc>

#### RectScreen
Draws a filled rectangle using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectScreen(Vec2 pos, Vec2 size, const std::string& hexcol);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height of the rectangle.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::RectScreen({0, 0}, {1920, 30}, "#000000FF");
```
</bloc>

<bloc>

#### RectOutlineWindow
Draws a rectangle outline using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectOutlineWindow(Vec2 pos, Vec2 size, const std::string& hexcol, float thickness = 1.0f);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height of the rectangle.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Outline thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::RectOutlineWindow({10, 10}, {200, 100}, "#FF0000FF", 2.0f);
```
</bloc>

<bloc>

#### RectOutlineScreen
Draws a rectangle outline using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectOutlineScreen(Vec2 pos, Vec2 size, const std::string& hexcol, float thickness = 1.0f);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height of the rectangle.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Outline thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::RectOutlineScreen({100, 100}, {400, 200}, "#FFFFFFFF");
```
</bloc>

<bloc>

#### RectOutlineRoundedWindow
Draws a rounded rectangle outline using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectOutlineRoundedWindow(Vec2 pos, Vec2 size, float rounding, const std::string& hexcol, float thickness);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height of the rectangle.
* **rounding**: Corner radius in pixels.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: Outline thickness in pixels.

**Example:**
```cpp
CherryDrawing::RectOutlineRoundedWindow({10, 10}, {200, 80}, 8.0f, "#FF0000FF", 2.0f);
```
</bloc>

<bloc>

#### RectOutlineRoundedScreen
Draws a rounded rectangle outline using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectOutlineRoundedScreen(Vec2 pos, Vec2 size, float rounding, const std::string& hexcol, float thickness);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height of the rectangle.
* **rounding**: Corner radius in pixels.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: Outline thickness in pixels.

**Example:**
```cpp
CherryDrawing::RectOutlineRoundedScreen({100, 100}, {400, 200}, 12.0f, "#FFFFFFFF", 1.5f);
```
</bloc>

<bloc>

#### RectRoundedWindow
Draws a filled rounded rectangle using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectRoundedWindow(Vec2 pos, Vec2 size, float rounding, const std::string& hexcol);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height of the rectangle.
* **rounding**: Corner radius in pixels.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::RectRoundedWindow({10, 10}, {200, 80}, 10.0f, "#3399FFFF");
```
</bloc>

<bloc>

#### RectRoundedScreen
Draws a filled rounded rectangle using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectRoundedScreen(Vec2 pos, Vec2 size, float rounding, const std::string& hexcol);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height of the rectangle.
* **rounding**: Corner radius in pixels.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::RectRoundedScreen({100, 100}, {300, 150}, 8.0f, "#222222FF");
```
</bloc>

<bloc>

#### RectRoundedOutlineWindow
Draws a rounded rectangle outline using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectRoundedOutlineWindow(Vec2 pos, Vec2 size, float rounding, const std::string& hexcol, float thickness = 1.0f);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height.
* **rounding**: Corner radius in pixels.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Outline thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::RectRoundedOutlineWindow({10, 10}, {200, 80}, 10.0f, "#FF0000FF", 2.0f);
```
</bloc>

<bloc>

#### RectRoundedOutlineScreen
Draws a rounded rectangle outline using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectRoundedOutlineScreen(Vec2 pos, Vec2 size, float rounding, const std::string& hexcol, float thickness = 1.0f);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height.
* **rounding**: Corner radius in pixels.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Outline thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::RectRoundedOutlineScreen({100, 100}, {300, 150}, 8.0f, "#FFFFFFFF");
```
</bloc>

<bloc>

#### SquareWindow
Draws a filled square using relative window coordinates. Shorthand for `RectWindow` with equal width and height.

**Signature and Parameters:**
```cpp
CherryDrawing::SquareWindow(Vec2 pos, float size, const std::string& hexcol);
```

* **pos**: Top-left corner (relative to window).
* **size**: Side length of the square.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::SquareWindow({50, 50}, 40.0f, "#FF9900FF");
```
</bloc>

<bloc>

#### SquareScreen
Draws a filled square using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::SquareScreen(Vec2 pos, float size, const std::string& hexcol);
```

* **pos**: Absolute top-left corner.
* **size**: Side length of the square.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::SquareScreen({200, 200}, 60.0f, "#FF9900FF");
```
</bloc>

<bloc>

#### QuadWindow
Draws a filled arbitrary quadrilateral defined by 4 points, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::QuadWindow(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const std::string& hexcol);
```

* **p0, p1, p2, p3**: The four vertices of the quad, in order (relative to window).
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::QuadWindow({0,0}, {100,10}, {90,100}, {0,90}, "#00FF00FF");
```
</bloc>

<bloc>

#### QuadScreen
Draws a filled arbitrary quadrilateral defined by 4 points using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::QuadScreen(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, const std::string& hexcol);
```

* **p0, p1, p2, p3**: Absolute screen positions of the four vertices, in order.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::QuadScreen({100,100}, {300,110}, {290,300}, {80,280}, "#00FF00FF");
```
</bloc>

<bloc>

#### TextWindow
Draws text at a position relative to the current window using the active font.

**Signature and Parameters:**
```cpp
CherryDrawing::TextWindow(const std::string& text, Vec2 pos, float size, const std::string& hexcol);
```

* **text**: The string to render.
* **pos**: Position of the top-left of the text (relative to window).
* **size**: Font size in pixels.
* **hexcol**: Text color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::TextWindow("Hello World", {10, 10}, 16.0f, "#FFFFFFFF");
```
</bloc>

<bloc>

#### TextScreen
Draws text at an absolute screen position using the active font.

**Signature and Parameters:**
```cpp
CherryDrawing::TextScreen(const std::string& text, Vec2 pos, float size, const std::string& hexcol);
```

* **text**: The string to render.
* **pos**: Absolute screen position of the text.
* **size**: Font size in pixels.
* **hexcol**: Text color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::TextScreen("Overlay text", {20, 20}, 14.0f, "#FFFF00FF");
```
</bloc>

<bloc>

#### CircleWindow
Draws a filled circle relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleWindow(Vec2 center, float radius, const std::string& hexcol, int num_segments = 0);
```

* **center**: Center of the circle (relative to window).
* **radius**: Radius in pixels.
* **hexcol**: Fill color in hexadecimal format.
* **num_segments**: *(optional)* Number of segments used to approximate the circle. `0` = automatic. Default: `0`.

**Example:**
```cpp
CherryDrawing::CircleWindow({100, 100}, 40.0f, "#FF0000FF");
```
</bloc>

<bloc>

#### CircleScreen
Draws a filled circle using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleScreen(Vec2 center, float radius, const std::string& hexcol, int num_segments = 0);
```

* **center**: Absolute screen center.
* **radius**: Radius in pixels.
* **hexcol**: Fill color in hexadecimal format.
* **num_segments**: *(optional)* Segment count. `0` = automatic. Default: `0`.

**Example:**
```cpp
CherryDrawing::CircleScreen({960, 540}, 50.0f, "#0099FFFF");
```
</bloc>

<bloc>

#### CircleOutlineWindow
Draws a circle outline relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleOutlineWindow(Vec2 center, float radius, const std::string& hexcol, float thickness = 1.0f, int num_segments = 0);
```

* **center**: Center of the circle (relative to window).
* **radius**: Radius in pixels.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Outline thickness. Default: `1.0f`.
* **num_segments**: *(optional)* Segment count. `0` = automatic. Default: `0`.

**Example:**
```cpp
CherryDrawing::CircleOutlineWindow({100, 100}, 40.0f, "#FF0000FF", 2.0f);
```
</bloc>

<bloc>

#### CircleOutlineScreen
Draws a circle outline using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleOutlineScreen(Vec2 center, float radius, const std::string& hexcol, float thickness = 1.0f, int num_segments = 0);
```

* **center**: Absolute screen center.
* **radius**: Radius in pixels.
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Outline thickness. Default: `1.0f`.
* **num_segments**: *(optional)* Segment count. `0` = automatic. Default: `0`.

**Example:**
```cpp
CherryDrawing::CircleOutlineScreen({960, 540}, 80.0f, "#FFFFFFFF", 1.5f);
```
</bloc>

<bloc>

#### CircleSectorWindow
Draws a filled circle sector (pie slice) between two angles, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleSectorWindow(Vec2 center, float radius, float angle_min, float angle_max, const std::string& hexcol, int num_segments = 0);
```

* **center**: Center of the arc (relative to window).
* **radius**: Radius in pixels.
* **angle_min**: Start angle in radians.
* **angle_max**: End angle in radians.
* **hexcol**: Fill color in hexadecimal format.
* **num_segments**: *(optional)* Segment count. `0` = automatic. Default: `0`.

**Example:**
```cpp
CherryDrawing::CircleSectorWindow({100, 100}, 50.0f, 0.0f, 1.5708f, "#FF9900FF");
```
</bloc>

<bloc>

#### CircleSectorScreen
Draws a filled circle sector (pie slice) between two angles using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleSectorScreen(Vec2 center, float radius, float angle_min, float angle_max, const std::string& hexcol, int num_segments = 0);
```

* **center**: Absolute screen center.
* **radius**: Radius in pixels.
* **angle_min**: Start angle in radians.
* **angle_max**: End angle in radians.
* **hexcol**: Fill color in hexadecimal format.
* **num_segments**: *(optional)* Segment count. `0` = automatic. Default: `0`.

**Example:**
```cpp
CherryDrawing::CircleSectorScreen({960, 540}, 100.0f, 0.0f, 3.14159f, "#FF0000FF");
```
</bloc>

<bloc>

#### TriangleWindow
Draws a filled triangle defined by 3 points, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleWindow(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& hexcol);
```

* **p1, p2, p3**: The three vertices of the triangle (relative to window).
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::TriangleWindow({50, 100}, {100, 0}, {150, 100}, "#FF0000FF");
```
</bloc>

<bloc>

#### TriangleScreen
Draws a filled triangle defined by 3 points using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleScreen(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& hexcol);
```

* **p1, p2, p3**: Absolute screen positions of the three vertices.
* **hexcol**: Fill color in hexadecimal format.

**Example:**
```cpp
CherryDrawing::TriangleScreen({400, 300}, {500, 100}, {600, 300}, "#00FF00FF");
```
</bloc>

<bloc>

#### TriangleOutlineWindow
Draws a triangle outline defined by 3 points, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleOutlineWindow(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& hexcol, float thickness = 1.0f);
```

* **p1, p2, p3**: The three vertices (relative to window).
* **hexcol**: Outline color in hexadecimal format.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::TriangleOutlineWindow({50, 100}, {100, 0}, {150, 100}, "#FF0000FF", 2.0f);
```
</bloc>

<bloc>

#### ImageWindow
Draws a textured image rectangle using relative window coordinates, with optional UV cropping and tint color.

**Signature and Parameters:**
```cpp
CherryDrawing::ImageWindow(Vec2 pos, Vec2 size, const std::string& path, Vec2 uv_min = {0,0}, Vec2 uv_max = {1,1}, const std::string& hexcol = "#FFFFFFFF");
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height of the rendered image.
* **path**: Path to the image file (loaded via `Cherry::GetTexture`).
* **uv_min**: *(optional)* Top-left UV coordinate for cropping. Default: `{0, 0}`.
* **uv_max**: *(optional)* Bottom-right UV coordinate for cropping. Default: `{1, 1}`.
* **hexcol**: *(optional)* Tint color. Default: `"#FFFFFFFF"` (no tint).

**Example:**
```cpp
CherryDrawing::ImageWindow({10, 10}, {128, 128}, "assets/icon.png");
CherryDrawing::ImageWindow({10, 10}, {128, 128}, "assets/icon.png", {0,0}, {0.5f,0.5f}, "#FFFFFF88");
```
</bloc>

<bloc>

#### ImageScreen
Draws a textured image rectangle using absolute screen coordinates, with optional UV cropping and tint color.

**Signature and Parameters:**
```cpp
CherryDrawing::ImageScreen(Vec2 pos, Vec2 size, const std::string& path, Vec2 uv_min = {0,0}, Vec2 uv_max = {1,1}, const std::string& hexcol = "#FFFFFFFF");
```

* **pos**: Absolute top-left corner.
* **size**: Width and height of the rendered image.
* **path**: Path to the image file.
* **uv_min**: *(optional)* Top-left UV coordinate. Default: `{0, 0}`.
* **uv_max**: *(optional)* Bottom-right UV coordinate. Default: `{1, 1}`.
* **hexcol**: *(optional)* Tint color. Default: `"#FFFFFFFF"`.

**Example:**
```cpp
CherryDrawing::ImageScreen({0, 0}, {1920, 1080}, "assets/background.png");
```
</bloc>

<bloc>

#### ImageQuadWindow
Draws a texture mapped onto an arbitrary quadrilateral using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::ImageQuadWindow(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, const std::string& path, const std::string& hexcol = "#FFFFFFFF");
```

* **p1, p2, p3, p4**: The four vertices of the quad in order: top-left, top-right, bottom-right, bottom-left (relative to window).
* **path**: Path to the image file.
* **hexcol**: *(optional)* Tint color. Default: `"#FFFFFFFF"`.

**Example:**
```cpp
CherryDrawing::ImageQuadWindow({0,0}, {100,10}, {90,100}, {0,90}, "assets/tile.png");
```
</bloc>

<bloc>

#### ImageQuadScreen
Draws a texture mapped onto an arbitrary quadrilateral using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::ImageQuadScreen(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, const std::string& path, const std::string& hexcol = "#FFFFFFFF");
```

* **p1, p2, p3, p4**: Absolute screen positions of the four quad vertices, in order.
* **path**: Path to the image file.
* **hexcol**: *(optional)* Tint color. Default: `"#FFFFFFFF"`.

**Example:**
```cpp
CherryDrawing::ImageQuadScreen({100,100}, {300,110}, {290,300}, {80,280}, "assets/tile.png");
```
</bloc>

<bloc>

#### ImageRoundedWindow
Draws a texture in a rounded rectangle using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::ImageRoundedWindow(Vec2 pos, Vec2 size, const std::string& path, float rounding, const std::string& hexcol = "#FFFFFFFF");
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height.
* **path**: Path to the image file.
* **rounding**: Corner radius in pixels.
* **hexcol**: *(optional)* Tint color. Default: `"#FFFFFFFF"`.

**Example:**
```cpp
CherryDrawing::ImageRoundedWindow({10, 10}, {128, 128}, "assets/avatar.png", 12.0f);
```
</bloc>

<bloc>

#### ImageRoundedScreen
Draws a texture in a rounded rectangle using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::ImageRoundedScreen(Vec2 pos, Vec2 size, const std::string& path, float rounding, const std::string& hexcol = "#FFFFFFFF");
```

* **pos**: Absolute top-left corner.
* **size**: Width and height.
* **path**: Path to the image file.
* **rounding**: Corner radius in pixels.
* **hexcol**: *(optional)* Tint color. Default: `"#FFFFFFFF"`.

**Example:**
```cpp
CherryDrawing::ImageRoundedScreen({200, 200}, {256, 256}, "assets/avatar.png", 16.0f);
```
</bloc>

<bloc>

#### RectGradientWindow
Draws a filled rectangle with a linear gradient using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectGradientWindow(Vec2 pos, Vec2 size, const std::string& col_start, const std::string& col_end, bool vertical = true, int steps = 32);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height.
* **col_start**: Start color in hexadecimal format.
* **col_end**: End color in hexadecimal format.
* **vertical**: *(optional)* `true` = top-to-bottom gradient, `false` = left-to-right. Default: `true`.
* **steps**: *(optional)* Number of interpolation slices. Higher = smoother. Default: `32`.

**Example:**
```cpp
CherryDrawing::RectGradientWindow({10, 10}, {200, 100}, "#FF0000FF", "#0000FFFF");
CherryDrawing::RectGradientWindow({10, 10}, {200, 100}, "#FF0000FF", "#0000FFFF", false);
```
</bloc>

<bloc>

#### RectGradientScreen
Draws a filled rectangle with a linear gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectGradientScreen(Vec2 pos, Vec2 size, const std::string& col_start, const std::string& col_end, bool vertical = true, int steps = 32);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height.
* **col_start**: Start color in hexadecimal format.
* **col_end**: End color in hexadecimal format.
* **vertical**: *(optional)* `true` = top-to-bottom, `false` = left-to-right. Default: `true`.
* **steps**: *(optional)* Interpolation slices. Default: `32`.

**Example:**
```cpp
CherryDrawing::RectGradientScreen({0, 0}, {1920, 1080}, "#000000FF", "#222244FF");
```
</bloc>

<bloc>

#### RectGradientRoundedWindow
Draws a filled rounded rectangle with a linear gradient using relative window coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectGradientRoundedWindow(Vec2 pos, Vec2 size, const std::string& hexcol_start, const std::string& hexcol_end, float rounding, bool vertical);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height.
* **hexcol_start**: Start color in hexadecimal format.
* **hexcol_end**: End color in hexadecimal format.
* **rounding**: Corner radius in pixels.
* **vertical**: `true` = top-to-bottom gradient, `false` = left-to-right.

**Example:**
```cpp
CherryDrawing::RectGradientRoundedWindow({10, 10}, {200, 80}, "#FF0000FF", "#0000FFFF", 10.0f, true);
```
</bloc>

<bloc>

#### RectGradientRoundedScreen
Draws a filled rounded rectangle with a linear gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectGradientRoundedScreen(Vec2 pos, Vec2 size, const std::string& hexcol_start, const std::string& hexcol_end, float rounding, bool vertical);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height.
* **hexcol_start**: Start color in hexadecimal format.
* **hexcol_end**: End color in hexadecimal format.
* **rounding**: Corner radius in pixels.
* **vertical**: `true` = top-to-bottom gradient, `false` = left-to-right.

**Example:**
```cpp
CherryDrawing::RectGradientRoundedScreen({100, 100}, {300, 150}, "#112233FF", "#334455FF", 8.0f, false);
```
</bloc>

<bloc>

#### CircleGradientWindow
Draws a radial gradient circle from center color to edge color, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleGradientWindow(Vec2 center, float radius, const std::string& col_center, const std::string& col_edge, int steps = 16);
```

* **center**: Center of the circle (relative to window).
* **radius**: Radius in pixels.
* **col_center**: Color at the center.
* **col_edge**: Color at the edge.
* **steps**: *(optional)* Number of concentric layers. Higher = smoother. Default: `16`.

**Example:**
```cpp
CherryDrawing::CircleGradientWindow({100, 100}, 60.0f, "#FFFFFFFF", "#00000000");
```
</bloc>

<bloc>

#### CircleGradientScreen
Draws a radial gradient circle from center color to edge color using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleGradientScreen(Vec2 center, float radius, const std::string& col_center, const std::string& col_edge, int steps = 16);
```

* **center**: Absolute screen center.
* **radius**: Radius in pixels.
* **col_center**: Color at the center.
* **col_edge**: Color at the edge.
* **steps**: *(optional)* Number of concentric layers. Default: `16`.

**Example:**
```cpp
CherryDrawing::CircleGradientScreen({960, 540}, 100.0f, "#FF9900FF", "#00000000", 32);
```
</bloc>

<bloc>

#### TriangleGradientWindow
Draws a filled triangle with a per-vertex color gradient, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& c1, const std::string& c2, const std::string& c3);
```

* **p1, p2, p3**: The three vertices (relative to window).
* **c1, c2, c3**: Colors assigned to each respective vertex.

**Example:**
```cpp
CherryDrawing::TriangleGradientWindow({50,100}, {100,0}, {150,100}, "#FF0000FF", "#00FF00FF", "#0000FFFF");
```
</bloc>

<bloc>

#### TriangleGradientScreen
Draws a filled triangle with per-vertex color gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& c1, const std::string& c2, const std::string& c3);
```

* **p1, p2, p3**: Absolute screen positions of the three vertices.
* **c1, c2, c3**: Colors assigned to each respective vertex.

**Example:**
```cpp
CherryDrawing::TriangleGradientScreen({400,300}, {500,100}, {600,300}, "#FF0000FF", "#00FF00FF", "#0000FFFF");
```
</bloc>

<bloc>

#### TriangleOutlineGradientWindow
Draws a triangle outline with a two-color gradient across its edges, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleOutlineGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& c1, const std::string& c2, float thickness = 1.0f);
```

* **p1, p2, p3**: The three vertices (relative to window).
* **c1**: Color of the first edge (p1→p2).
* **c2**: Color of the last edge (p3→p1). The middle edge interpolates between the two.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::TriangleOutlineGradientWindow({50,100}, {100,0}, {150,100}, "#FF0000FF", "#0000FFFF", 2.0f);
```
</bloc>

<bloc>

#### TriangleOutlineGradientScreen
Draws a triangle outline with a two-color gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::TriangleOutlineGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, const std::string& c1, const std::string& c2, float thickness = 1.0f);
```

* **p1, p2, p3**: Absolute screen positions of the three vertices.
* **c1**: Color of the first edge.
* **c2**: Color of the last edge. The middle edge interpolates.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::TriangleOutlineGradientScreen({400,300}, {500,100}, {600,300}, "#FF0000FF", "#0000FFFF");
```
</bloc>

<bloc>

#### QuadGradientWindow
Draws a filled quadrilateral with a per-vertex color gradient, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::QuadGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, const std::string& c1, const std::string& c2, const std::string& c3, const std::string& c4);
```

* **p1, p2, p3, p4**: The four vertices in order (relative to window).
* **c1, c2, c3, c4**: Colors assigned to each respective vertex.

**Example:**
```cpp
CherryDrawing::QuadGradientWindow({0,0}, {100,0}, {100,100}, {0,100}, "#FF0000FF", "#00FF00FF", "#0000FFFF", "#FFFF00FF");
```
</bloc>

<bloc>

#### QuadGradientScreen
Draws a filled quadrilateral with a per-vertex color gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::QuadGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, const std::string& c1, const std::string& c2, const std::string& c3, const std::string& c4);
```

* **p1, p2, p3, p4**: Absolute screen positions of the four vertices in order.
* **c1, c2, c3, c4**: Colors assigned to each respective vertex.

**Example:**
```cpp
CherryDrawing::QuadGradientScreen({100,100}, {300,100}, {300,300}, {100,300}, "#FF0000FF", "#00FF00FF", "#0000FFFF", "#FFFF00FF");
```
</bloc>

<bloc>

#### RectOutlineGradientWindow
Draws a rectangle outline with a two-color gradient across its edges, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::RectOutlineGradientWindow(Vec2 pos, Vec2 size, const std::string& col_start, const std::string& col_end, float thickness = 1.0f);
```

* **pos**: Top-left corner (relative to window).
* **size**: Width and height.
* **col_start**: Color of the top edge.
* **col_end**: Color of the bottom edge. Side edges interpolate.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::RectOutlineGradientWindow({10, 10}, {200, 100}, "#FF0000FF", "#0000FFFF", 2.0f);
```
</bloc>

<bloc>

#### RectOutlineGradientScreen
Draws a rectangle outline with a two-color gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::RectOutlineGradientScreen(Vec2 pos, Vec2 size, const std::string& col_start, const std::string& col_end, float thickness = 1.0f);
```

* **pos**: Absolute top-left corner.
* **size**: Width and height.
* **col_start**: Color of the top edge.
* **col_end**: Color of the bottom edge. Side edges interpolate.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::RectOutlineGradientScreen({100, 100}, {400, 200}, "#FF00FFFF", "#00FFFFFF");
```
</bloc>

<bloc>

#### CircleOutlineGradientWindow
Draws a circle outline with a gradient that rotates around the circumference, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleOutlineGradientWindow(Vec2 center, float radius, const std::string& col_start, const std::string& col_end, float thickness = 1.0f);
```

* **center**: Center of the circle (relative to window).
* **radius**: Radius in pixels.
* **col_start**: Color at the start of the arc (angle 0).
* **col_end**: Color at the end of the arc (full revolution). Uses 64 segments internally.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::CircleOutlineGradientWindow({100, 100}, 50.0f, "#FF0000FF", "#0000FFFF", 2.0f);
```
</bloc>

<bloc>

#### CircleOutlineGradientScreen
Draws a circle outline with a rotating gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::CircleOutlineGradientScreen(Vec2 center, float radius, const std::string& col_start, const std::string& col_end, float thickness = 1.0f);
```

* **center**: Absolute screen center.
* **radius**: Radius in pixels.
* **col_start**: Color at the start of the arc.
* **col_end**: Color at the end of the arc. Uses 64 segments internally.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::CircleOutlineGradientScreen({960, 540}, 80.0f, "#FF00FFFF", "#00FFFFFF");
```
</bloc>

<bloc>

#### QuadOutlineGradientWindow
Draws a quadrilateral outline with a two-color gradient across its edges, relative to the current window.

**Signature and Parameters:**
```cpp
CherryDrawing::QuadOutlineGradientWindow(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, const std::string& c1, const std::string& c2, float thickness = 1.0f);
```

* **p1, p2, p3, p4**: The four vertices in order (relative to window).
* **c1**: Color applied to the first two edges (p1→p2, p2→p3).
* **c2**: Color applied to the last two edges (p3→p4, p4→p1).
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::QuadOutlineGradientWindow({0,0}, {100,0}, {100,100}, {0,100}, "#FF0000FF", "#0000FFFF");
```
</bloc>

<bloc>

#### QuadOutlineGradientScreen
Draws a quadrilateral outline with a two-color gradient using absolute screen coordinates.

**Signature and Parameters:**
```cpp
CherryDrawing::QuadOutlineGradientScreen(Vec2 p1, Vec2 p2, Vec2 p3, Vec2 p4, const std::string& c1, const std::string& c2, float thickness = 1.0f);
```

* **p1, p2, p3, p4**: Absolute screen positions of the four vertices in order.
* **c1**: Color of the first two edges.
* **c2**: Color of the last two edges.
* **thickness**: *(optional)* Line thickness. Default: `1.0f`.

**Example:**
```cpp
CherryDrawing::QuadOutlineGradientScreen({100,100}, {300,120}, {280,300}, {80,280}, "#FF00FFFF", "#00FFFFFF");
```
</bloc>

<bloc>

#### BeginRectMask
Begins a rectangular clipping mask in relative window coordinates. All subsequent window draw calls are clipped to this region until `EndRectMask` is called.

**Signature and Parameters:**
```cpp
CherryDrawing::BeginRectMask(Vec2 pos, Vec2 size, bool intersect_with_current_clip_rect = true);
```

* **pos**: Top-left corner of the mask (relative to window).
* **size**: Width and height of the mask region.
* **intersect_with_current_clip_rect**: *(optional)* Whether to intersect with any existing clip rect. Default: `true`.

**Example:**
```cpp
CherryDrawing::BeginRectMask({10, 10}, {200, 150});
// masked drawing calls
CherryDrawing::EndRectMask();
```
</bloc>

<bloc>

#### EndRectMask
Ends the rectangular clipping mask started by `BeginRectMask`. Must always be called after a `BeginRectMask`.

**Signature and Parameters:**
```cpp
CherryDrawing::EndRectMask();
```

**Example:**
```cpp
CherryDrawing::BeginRectMask({10, 10}, {200, 150});
// masked drawing calls
CherryDrawing::EndRectMask();
```
</bloc>

<bloc>

#### BeginRectMaskFixed
Begins a rectangular clipping mask using absolute screen coordinates. All subsequent foreground draw calls are clipped to this region until `EndRectMaskFixed` is called.

**Signature and Parameters:**
```cpp
CherryDrawing::BeginRectMaskFixed(Vec2 pos, Vec2 size, bool intersect_with_current_clip_rect = true);
```

* **pos**: Absolute top-left corner of the mask.
* **size**: Width and height of the mask region.
* **intersect_with_current_clip_rect**: *(optional)* Whether to intersect with any existing clip rect. Default: `true`.

**Example:**
```cpp
CherryDrawing::BeginRectMaskFixed({100, 100}, {400, 300});
// masked drawing calls
CherryDrawing::EndRectMaskFixed();
```
</bloc>

<bloc>

#### EndRectMaskFixed
Ends the rectangular clipping mask started by `BeginRectMaskFixed`. Must always be called after a `BeginRectMaskFixed`.

**Signature and Parameters:**
```cpp
CherryDrawing::EndRectMaskFixed();
```

**Example:**
```cpp
CherryDrawing::BeginRectMaskFixed({100, 100}, {400, 300});
// masked drawing calls
CherryDrawing::EndRectMaskFixed();
```
</bloc>