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
#### DrawRectOutlineFixed
#### DrawText
#### DrawTextFixed
#### DrawLine
#### DrawLineFixed
#### DrawLineHorizontal
#### DrawLineHorizontalFixed
#### DrawLineVertical
#### DrawLineVerticalFixed
#### DrawPolyline
#### DrawPolylineFixed
#### DrawCross
#### DrawCrossFixed
#### DrawSquare
#### DrawSquareFixed
#### DrawQuad
#### DrawQuadFixed
#### DrawCircle
#### DrawCircleFixed
#### DrawCircleOutline
#### DrawCircleOutlineFixed
#### DrawTriangle
#### DrawTriangleFixed
#### DrawRectRounded
#### DrawRectRoundedFixed
#### DrawImageRounded
#### DrawImageRoundedFixed
#### DrawImage
#### DrawImageFixed
#### DrawImageQuad
#### DrawImageQuadFixed
#### DrawRectGradient
#### DrawRectGradientFixed
#### DrawRectOutlineGradient
#### DrawRectOutlineGradientFixed
#### DrawCircleGradient
#### DrawCircleGradientFixed
#### DrawCircleOutlineGradient
#### DrawCircleOutlineGradientFixed
#### DrawTriangleGradient
#### DrawTriangleGradientFixed
#### DrawTriangleOutlineGradient
#### DrawTriangleOutlineGradientFixed
#### DrawQuadGradient
#### DrawQuadGradientFixed
#### DrawQuadOutlineGradient
#### DrawQuadOutlineGradientFixed

### Logic API
This section provides functions for all logic features of Cherry.

#### GetCursorPos
#### GetCursorPosX
#### GetCursorPosY
#### GetWindowSize
#### GetWindowSizeX
#### GetWindowSizeY
#### GetWindowPos
#### GetWindowPosX
#### GetWindowPosY
#### GetPath
#### GetLocale
#### PushTheme
#### PopTheme
#### SetTheme
#### GetThemeProperty
#### GetActiveThemeProperty
#### Log
#### LogInfo
#### LogWarn
#### LogError
#### LogFatal
#### LogTrace
#### IsKeyPressed
#### PlaySound