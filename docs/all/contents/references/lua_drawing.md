### Drawing API
This section provides functions for drawing primitive shapes. Functions without a suffix use coordinates relative to the current window, while **Fixed** functions use absolute screen coordinates.

---

#### DrawRect
Draws a filled rectangle at the specified coordinates, relative to the window position.

**Signature and Parameters:**
```lua 
Cherry.DrawRect(x, y, width, height, hex_color)
```
* **x, y**: Position of the top-left corner (relative to window).
* **width, height**: Dimensions of the rectangle.
* **hex_color**: Color in hexadecimal format (e.g., "#FF0000FF").

**Example:**
```lua
Cherry.DrawRect(100, 100, 50, 50, "#FF0000FF")
```

---

#### DrawRectFixed
Draws a filled rectangle using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawRectFixed(x, y, width, height, hex_color)
```
* **x, y**: Absolute position on the screen (0,0 is the top-left corner).
* **width, height**: Dimensions of the rectangle.
* **hex_color**: Color in hexadecimal format.

**Example:**
```lua
Cherry.DrawRectFixed(0, 0, 1920, 30, "#000000FF")
```

---

#### DrawRectOutline
Draws the outline of a rectangle with a customizable line thickness, relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawRectOutline(x, y, width, height, hex_color, thickness)
```
* **x, y**: Position of the top-left corner (relative to window).
* **width, height**: Dimensions of the rectangle.
* **hex_color**: Color of the outline.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawRectOutline(45, 45, 110, 110, "#00AAFFFF", 2.0)
```

---

#### DrawRectOutlineFixed
Draws the outline of a rectangle using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawRectOutlineFixed(x, y, width, height, hex_color, thickness)
```
* **x, y**: Absolute position on the screen (0,0 is the top-left corner).
* **width, height**: Dimensions of the rectangle.
* **hex_color**: Color of the outline.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawRectOutlineFixed(10, 10, 200, 50, "#FFFFFFFF", 1.0)
```

---

#### DrawText
Draws a string of text relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawText(x, y, size, hex_color, text)
```
* **x, y**: Position of the text (relative to window).
* **size**: Font size or scaling factor.
* **hex_color**: Text color.
* **text**: The string of text to display.

**Example:**
```lua
Cherry.DrawText(20, 20, 18.0, "#FFFFFFCC", "Hello World")
```

---

#### DrawTextFixed
Draws a string of text using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawTextFixed(x, y, size, hex_color, text)
```
* **x, y**: Absolute position on the screen.
* **size**: Font size or scaling factor.
* **hex_color**: Text color.
* **text**: The string of text to display.

**Example:**
```lua
Cherry.DrawTextFixed(100, 100, 24.0, "#00FF00FF", "Screen Overlay")
```

---

#### DrawLine
Draws a line between two points relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawLine(start_x, start_y, end_x, end_y, hex_color, thickness)
```
* **start_x, start_y**: Starting coordinates.
* **end_x, end_y**: Ending coordinates.
* **hex_color**: Line color.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawLine(0, 0, 100, 100, "#FF0000FF", 2.0)
```

---

#### DrawLineFixed
Draws a line between two points using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawLineFixed(start_x, start_y, end_x, end_y, hex_color, thickness)
```
* **start_x, start_y**: Absolute starting coordinates.
* **end_x, end_y**: Absolute ending coordinates.
* **hex_color**: Line color.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawLineFixed(0, 0, 500, 500, "#FFFFFFFF", 1.0)
```

---

#### DrawLineHorizontal
Draws a horizontal line relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawLineHorizontal(y, start_x, end_x, hex_color, thickness)
```
* **y**: Vertical position of the line.
* **start_x**: Starting horizontal position.
* **end_x**: Ending horizontal position.
* **hex_color**: Line color.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawLineHorizontal(50, 10, 100, "#00AAFFFF", 1.0)
```

---

#### DrawLineHorizontalFixed
Draws a horizontal line using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawLineHorizontalFixed(y, start_x, end_x, hex_color, thickness)
```
* **y**: Absolute vertical position.
* **start_x**: Absolute starting horizontal position.
* **end_x**: Absolute ending horizontal position.
* **hex_color**: Line color.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawLineHorizontalFixed(1080 / 2, 0, 1920, "#FF00FFFF", 2.0)
```

---

#### DrawLineVertical
Draws a vertical line relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawLineVertical(x, start_y, end_y, hex_color, thickness)
```
* **x**: Horizontal position of the line.
* **start_y**: Starting vertical position.
* **end_y**: Ending vertical position.
* **hex_color**: Line color.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawLineVertical(50, 10, 100, "#00AAFFFF", 1.0)
```

---

#### DrawLineVerticalFixed
Draws a vertical line using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawLineVerticalFixed(x, start_y, end_y, hex_color, thickness)
```
* **x**: Absolute horizontal position.
* **start_y**: Absolute starting vertical position.
* **end_y**: Absolute ending vertical position.
* **hex_color**: Line color.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawLineVerticalFixed(1920 / 2, 0, 1080, "#FFFF00FF", 2.0)
```
---

#### DrawPolyline
Draws a sequence of connected line segments relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawPolyline(points_table, hex_color, thickness)
```
* **points_table**: A table of points, where each point is a table ``{x, y}``.
* **hex_color**: Color of the lines.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawPolyline({{10, 10}, {50, 20}, {90, 10}}, "#FFFFFFFF", 2.0)
```

---

#### DrawPolylineFixed
Draws a sequence of connected line segments using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawPolylineFixed(points_table, hex_color, thickness)
```
* **points_table**: A table of points ``{{x1, y1}, {x2, y2}, ...}``.
* **hex_color**: Color of the lines.
* **thickness**: Line thickness in pixels.

**Example:**
```lua
Cherry.DrawPolylineFixed({{0, 0}, {100, 100}, {200, 0}}, "#FF0000FF", 1.0)
```

---

#### DrawCross
Draws an "X" shaped cross centered at the specified position, relative to the window.

**Signature and Parameters:**
```lua
Cherry.DrawCross(x, y, size, hex_color, thickness)
```
* **x, y**: Center position of the cross.
* **size**: The span of the cross arms.
* **hex_color**: Color of the cross.
* **thickness**: Line thickness of the arms.

**Example:**
```lua
Cherry.DrawCross(100, 100, 20, "#FF0000FF", 2.0)
```

---

#### DrawCrossFixed
Draws an "X" shaped cross centered at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawCrossFixed(x, y, size, hex_color, thickness)
```
* **x, y**: Absolute center position.
* **size**: The span of the cross arms.
* **hex_color**: Color of the cross.
* **thickness**: Line thickness.

**Example:**
```lua
Cherry.DrawCrossFixed(960, 540, 50, "#00FF00FF", 1.5)
```

---

#### DrawSquare
Draws a filled square relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawSquare(x, y, size, hex_color)
```
* **x, y**: Top-left corner position.
* **size**: The length of each side.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawSquare(50, 50, 40, "#00AAFFFF")
```

---

#### DrawSquareFixed
Draws a filled square at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawSquareFixed(x, y, size, hex_color)
```
* **x, y**: Absolute top-left corner position.
* **size**: The length of each side.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawSquareFixed(10, 10, 100, "#222222FF")
```

---

#### DrawQuad
Draws a filled quadrilateral (4-sided polygon) relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawQuad(x1, y1, x2, y2, x3, y3, x4, y4, hex_color)
```
* **x1, y1...x4, y4**: Coordinates of the four vertices in order.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawQuad(10, 10, 100, 20, 90, 80, 20, 90, "#FFCC00FF")
```

---

#### DrawQuadFixed
Draws a filled quadrilateral at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawQuadFixed(x1, y1, x2, y2, x3, y3, x4, y4, hex_color)
```
* **x1, y1...x4, y4**: Absolute coordinates of the four vertices.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawQuadFixed(0, 0, 200, 50, 150, 200, 50, 150, "#FF00FFFF")
```

---

#### DrawCircle
Draws a filled circle relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawCircle(x, y, radius, hex_color, segments)
```
* **x, y**: Center position.
* **radius**: Circle radius.
* **hex_color**: Fill color.
* **segments**: Level of detail (0 for auto-calculation).

**Example:**
```lua
Cherry.DrawCircle(100, 100, 30, "#FF0000FF", 0)
```

---

#### DrawCircleFixed
Draws a filled circle at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawCircleFixed(x, y, radius, hex_color, segments)
```
* **x, y**: Absolute center position.
* **radius**: Circle radius.
* **hex_color**: Fill color.
* **segments**: Level of detail.

**Example:**
```lua
Cherry.DrawCircleFixed(960, 540, 100, "#FFFFFFFF", 64)
```

---

#### DrawCircleOutline
Draws a circle outline relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawCircleOutline(x, y, radius, hex_color, thickness, segments)
```
* **x, y**: Center position.
* **radius**: Circle radius.
* **hex_color**: Outline color.
* **thickness**: Line thickness.
* **segments**: Level of detail.

**Example:**
```lua
Cherry.DrawCircleOutline(100, 100, 30, "#00FF00FF", 2.0, 32)
```

---

#### DrawCircleOutlineFixed
Draws a circle outline at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawCircleOutlineFixed(x, y, radius, hex_color, thickness, segments)
```
* **x, y**: Absolute center position.
* **radius**: Circle radius.
* **hex_color**: Outline color.
* **thickness**: Line thickness.
* **segments**: Level of detail.

**Example:**
```lua
Cherry.DrawCircleOutlineFixed(500, 500, 50, "#FFAA00FF", 1.0, 0)
```

---

#### DrawTriangle
Draws a filled triangle relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawTriangle(p1x, p1y, p2x, p2y, p3x, p3y, hex_color)
```
* **p1, p2, p3**: Coordinates for the three vertices.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawTriangle(50, 10, 90, 90, 10, 90, "#00FF00FF")
```

---

#### DrawTriangleFixed
Draws a filled triangle at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawTriangleFixed(p1x, p1y, p2x, p2y, p3x, p3y, hex_color)
```
* **p1, p2, p3**: Absolute coordinates for the vertices.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawTriangleFixed(100, 100, 200, 100, 150, 200, "#FF0000FF")
```

---

#### DrawRectRounded
Draws a filled rectangle with rounded corners relative to the window position.

**Signature and Parameters:**
```lua
Cherry.DrawRectRounded(x, y, width, height, rounding, hex_color)
```
* **x, y**: Position of the top-left corner.
* **width, height**: Dimensions of the rectangle.
* **rounding**: Radius for the rounded corners.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawRectRounded(10, 10, 200, 100, 10.0, "#333333FF")
```

---

#### DrawRectRoundedFixed
Draws a filled rectangle with rounded corners using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawRectRoundedFixed(x, y, width, height, rounding, hex_color)
```
* **x, y**: Absolute screen position.
* **width, height**: Dimensions of the rectangle.
* **rounding**: Radius for the rounded corners.
* **hex_color**: Fill color.

**Example:**
```lua
Cherry.DrawRectRoundedFixed(0, 0, 400, 50, 5.0, "#FF0000FF")
```

---

#### DrawImage
Draws a standard image texture with optional UV mapping and tinting.

**Signature and Parameters:**
```lua
Cherry.DrawImage(x, y, w, h, path, [uv_min_x, uv_min_y, uv_max_x, uv_max_y, hex_color])
```
* **x, y, w, h**: Position and size.
* **path**: Resource path to the image file.
* **uv_min/uv_max**: Optional UV coordinates (default 0,0 to 1,1).
* **hex_color**: Optional tint color (default "#FFFFFFFF").

**Example:**
```lua
Cherry.DrawImage(50, 50, 128, 128, "textures/ui_icon.png", 0, 0, 1, 1, "#FFFFFF88")
```

---

#### DrawImageFixed
Draws a standard image texture using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawImageFixed(x, y, w, h, path, [uv_min_x, uv_min_y, uv_max_x, uv_max_y, hex_color])
```
* **path**: Resource path to the image file.
* **hex_color**: Optional tint color.

**Example:**
```lua
Cherry.DrawImageFixed(0, 0, 1920, 1080, "backgrounds/main.jpg")
```

---

#### DrawImageRounded
Draws an image texture clipped to a rounded rectangle.

**Signature and Parameters:**
```lua
Cherry.DrawImageRounded(x, y, w, h, rounding, path, [hex_color])
```
* **rounding**: Radius for the corners.
* **path**: Resource path to the image.
* **hex_color**: Optional tinting (default "#FFFFFFFF").

**Example:**
```lua
Cherry.DrawImageRounded(20, 20, 100, 100, 50.0, "avatars/user.png")
```

---

#### DrawImageRoundedFixed
Draws an image texture clipped to a rounded rectangle at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawImageRoundedFixed(x, y, w, h, rounding, path, [hex_color])
```
* **rounding**: Radius for the corners.
* **path**: Resource path to the image.

**Example:**
```lua
Cherry.DrawImageRoundedFixed(800, 20, 64, 64, 32.0, "icons/status.png")
```

---

#### DrawImageQuad
Draws an image texture mapped onto a custom 4-point quadrilateral.

**Signature and Parameters:**
```lua
Cherry.DrawImageQuad(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, path, [hex_color])
```
* **p1...p4**: Coordinates for the four corners.
* **path**: Resource path to the image.
* **hex_color**: Optional tinting.

**Example:**
```lua
Cherry.DrawImageQuad(10,10, 110,20, 100,120, 0,100, "textures/skewed.png")
```

---

#### DrawImageQuadFixed
Draws an image texture mapped onto a custom 4-point quadrilateral using absolute coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawImageQuadFixed(p1x, p1y, p2x, p2y, p3x, p3y, p4x, p4y, path, [hex_color])
```
* **path**: Resource path to the image.

**Example:**
```lua
Cherry.DrawImageQuadFixed(0,0, 200,0, 180,150, 20,150, "ui/header_skew.png")
```

---

#### DrawRectGradient
Draws a rectangle with a linear color gradient.

**Signature and Parameters:**
```lua
Cherry.DrawRectGradient(x, y, w, h, color_start, color_end, [vertical, steps])
```
* **color_start**: Initial color.
* **color_end**: Target color.
* **vertical**: Optional boolean (default true). If false, gradient is horizontal.
* **steps**: Optional detail level (default 32).

**Example:**
```lua
Cherry.DrawRectGradient(0, 0, 300, 50, "#FF0000FF", "#0000FFFF", false)
```

---

#### DrawRectGradientFixed
Draws a rectangle with a linear color gradient at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawRectGradientFixed(x, y, w, h, color_start, color_end, [vertical, steps])
```
* **color_start, color_end**: Gradient colors.

**Example:**
```lua
Cherry.DrawRectGradientFixed(0, 0, 1920, 2, "#FFFFFFFF", "#FFFFFF00")
```

---

#### DrawRectOutlineGradient
Draws the outline of a rectangle with a color gradient.

**Signature and Parameters:**
```lua
Cherry.DrawRectOutlineGradient(x, y, w, h, color_start, color_end, [thickness])
```
* **color_start, color_end**: Gradient colors.
* **thickness**: Optional line thickness (default 1.0).

**Example:**
```lua
Cherry.DrawRectOutlineGradient(50, 50, 100, 100, "#00FF00FF", "#002200FF", 2.5)
```

---

#### DrawRectOutlineGradientFixed
Draws the outline of a rectangle with a color gradient at absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawRectOutlineGradientFixed(x, y, w, h, color_start, color_end, [thickness])
```
* **thickness**: Optional line thickness.

**Example:**
```lua
Cherry.DrawRectOutlineGradientFixed(10, 10, 500, 500, "#FFFFFF88", "#FFFFFF00", 1.0)
```

---

#### DrawCircleGradient
Draws a filled circle with a radial gradient from the center to the edge, relative to the window.

**Signature and Parameters:**
```lua
Cherry.DrawCircleGradient(x, y, radius, color_center, color_edge, [steps])
```
* **x, y**: Center position.
* **radius**: Circle radius.
* **color_center**: Color at the center point.
* **color_edge**: Color at the outer circumference.
* **steps**: Optional detail level for the radial rings (default 16).

**Example:**
```lua
Cherry.DrawCircleGradient(100, 100, 50, "#FFFFFFFF", "#00000000", 32)
```

---

#### DrawCircleGradientFixed
Draws a filled circle with a radial gradient using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawCircleGradientFixed(x, y, radius, color_center, color_edge, [steps])
```

**Example:**
```lua
Cherry.DrawCircleGradientFixed(960, 540, 200, "#FFCC00FF", "#FF000000")
```

---

#### DrawCircleOutlineGradient
Draws a circle outline with a color gradient between two points, relative to the window.

**Signature and Parameters:**
```lua
Cherry.DrawCircleOutlineGradient(x, y, radius, color_start, color_end, [thickness])
```
* **color_start, color_end**: Colors used for the outline interpolation.
* **thickness**: Optional line thickness (default 1.0).

**Example:**
```lua
Cherry.DrawCircleOutlineGradient(100, 100, 40, "#00FFFFFF", "#004444FF", 2.0)
```

---

#### DrawCircleOutlineGradientFixed
Draws a circle outline with a color gradient using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawCircleOutlineGradientFixed(x, y, radius, color_start, color_end, [thickness])
```

**Example:**
```lua
Cherry.DrawCircleOutlineGradientFixed(500, 500, 100, "#FFFFFFFF", "#FF0000FF", 3.0)
```

---

#### DrawTriangleGradient
Draws a filled triangle with a unique color at each vertex, creating a multi-color interpolation.

**Signature and Parameters:**
```lua
Cherry.DrawTriangleGradient(x1, y1, x2, y2, x3, y3, c1, c2, c3)
```
* **x, y (1-3)**: Coordinates for the three vertices.
* **c1, c2, c3**: Colors assigned to vertex 1, 2, and 3 respectively.

**Example:**
```lua
Cherry.DrawTriangleGradient(100, 20, 180, 150, 20, 150, "#FF0000FF", "#00FF00FF", "#0000FFFF")
```

---

#### DrawTriangleGradientFixed
Draws a multi-color filled triangle using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawTriangleGradientFixed(x1, y1, x2, y2, x3, y3, c1, c2, c3)
```

**Example:**
```lua
Cherry.DrawTriangleGradientFixed(0, 0, 100, 0, 50, 100, "#FFFFFFFF", "#FF00FFFF", "#00FFFFFF")
```

---

#### DrawTriangleOutlineGradient
Draws a triangle outline with a gradient between vertices, relative to the window.

**Signature and Parameters:**
```lua
Cherry.DrawTriangleOutlineGradient(x1, y1, x2, y2, x3, y3, c1, c2, [thickness])
```
* **c1, c2**: Start and end colors for the outline interpolation.
* **thickness**: Optional line thickness (default 1.0).

**Example:**
```lua
Cherry.DrawTriangleOutlineGradient(50, 50, 100, 100, 0, 100, "#FF0000FF", "#FFFF00FF", 2.0)
```

---

#### DrawTriangleOutlineGradientFixed
Draws a triangle outline with a gradient using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawTriangleOutlineGradientFixed(x1, y1, x2, y2, x3, y3, c1, c2, [thickness])
```

**Example:**
```lua
Cherry.DrawTriangleOutlineGradientFixed(960, 100, 1000, 150, 920, 150, "#00FF00FF", "#00000000")
```

---

#### DrawQuadGradient
Draws a filled quadrilateral with a unique color at each of its four corners.

**Signature and Parameters:**
```lua
Cherry.DrawQuadGradient(x1, y1, x2, y2, x3, y3, x4, y4, c1, c2, c3, c4)
```
* **x, y (1-4)**: Coordinates for the four corners.
* **c1, c2, c3, c4**: Colors assigned to each corner vertex.

**Example:**
```lua
Cherry.DrawQuadGradient(0,0, 100,0, 100,100, 0,100, "#FF0000FF", "#FFFF00FF", "#00FF00FF", "#0000FFFF")
```

---

#### DrawQuadGradientFixed
Draws a multi-color filled quadrilateral using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawQuadGradientFixed(x1, y1, x2, y2, x3, y3, x4, y4, c1, c2, c3, c4)
```

**Example:**
```lua
Cherry.DrawQuadGradientFixed(0, 0, 1920, 0, 1920, 1080, 0, 1080, "#000000FF", "#222222FF", "#111111FF", "#000000FF")
```

---

#### DrawQuadOutlineGradient
Draws a quadrilateral outline with a color gradient, relative to the window.

**Signature and Parameters:**
```lua
Cherry.DrawQuadOutlineGradient(x1, y1, x2, y2, x3, y3, x4, y4, c1, c2, [thickness])
```
* **c1, c2**: Gradient colors for the outline.
* **thickness**: Optional line thickness (default 1.0).

**Example:**
```lua
Cherry.DrawQuadOutlineGradient(10, 10, 110, 10, 110, 110, 10, 110, "#FFFFFF88", "#FFFFFF00", 1.5)
```

---

#### DrawQuadOutlineGradientFixed
Draws a quadrilateral outline with a color gradient using absolute screen coordinates.

**Signature and Parameters:**
```lua
Cherry.DrawQuadOutlineGradientFixed(x1, y1, x2, y2, x3, y3, x4, y4, c1, c2, [thickness])
```

**Example:**
```lua
Cherry.DrawQuadOutlineGradientFixed(100, 100, 300, 120, 280, 300, 80, 280, "#FF00FFFF", "#00FFFFFF")
```