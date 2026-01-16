local label = Cherry.GetCurrentComponentProperty("label")
local x, y = Cherry.GetDrawCursorPos()
local w, h = 80, 60
local mx, my = Cherry.GetMousePos()

local hovered = mx >= x and mx <= x + w and my >= y and my <= y + h
local clicked = Cherry.IsMouseClickedOnPos(x, y, w, h, 0)
local isDown = (hovered and Cherry.IsKeyPressedOnPos(x, y, w, h, "MouseLeft")) or clicked

Cherry.SetCurrentComponentData("isClicked", clicked and "true" or "false")

local colTop, colBot = "#444444FF", "#222222FF"
if label == "C" then 
    colTop, colBot = "#E53935FF", "#8B0000FF"
elseif label:match("[%+%-%*/=]") then 
    colTop, colBot = "#FB8C00FF", "#BF360CFF" 
end

if isDown then
    Cherry.DrawRectRounded(x, y, w, h, 8, "#000000AA")
    Cherry.DrawRectGradientRounded(x + 1, y + 2, w - 2, h - 2, colBot, colTop, 8, true)
else
    Cherry.DrawRectRounded(x, y + 6, w, h, 8, "#00000077")
    Cherry.DrawRectRounded(x, y, w, h + 4, 8, colBot)
    Cherry.DrawRectGradientRounded(x, y, w, h, colTop, colBot, 8,true)
    Cherry.DrawRectOutlineRounded(x + 1, y + 1, w - 2, h - 2, 8, "#FFFFFF33", 1)
    Cherry.DrawLineHorizontal(y + 2, x + 10, x + w - 10, "#FFFFFF22", 2)
end

local wx, wy = Cherry.GetWindowPos()
local tx, ty = x + (w * 0.5),  y + (h * 0.5) - 12

if isDown then 
    tx, ty = tx + 1, ty + 2 
else
    Cherry.DrawText(tx - 7, ty + 1, 24, "#00000099", label)
end

local fontOffset = #label > 1 and 15 or 8
Cherry.DrawText(tx - fontOffset, ty, 24, "#FFFFFFFF", label)