Cherry.Use("Button", {
    ["prop:width"]  = "120",
    ["prop:height"] = "32",
    ["prop:color"]  = "#4444AA",
    ["prop:hover"]  = "#6666CC",
    ["prop:active"] = "#222288",
    ["prop:label"]  = "Click me"
}, function()
    local w     = tonumber(Cherry.GetCurrentComponentProperty("width"))  or 120
    local h     = tonumber(Cherry.GetCurrentComponentProperty("height")) or 32
    local color = Cherry.GetCurrentComponentProperty("color")
    local hover = Cherry.GetCurrentComponentProperty("hover")
    local active= Cherry.GetCurrentComponentProperty("active")
    local label = Cherry.GetCurrentComponentProperty("label") or "Button"

    local x = Cherry.GetDrawCursorPosX()
    local y = Cherry.GetDrawCursorPosY()

    local isClicked = Cherry.GetCurrentComponentData("isClicked") or false


    local isHover = Cherry.IsMouseOverRect(x, y, w, h)
    local clicked = Cherry.IsMouseClickedOnPos(x, y, w, h, 1)


    Cherry.SetCurrentComponentData("isClicked", "false")
    if clicked then
        isClicked = true
        Cherry.SetCurrentComponentData("isClicked", "true")
    end

    local drawColor = color
    if isHover then
        drawColor = hover
    end
    if isHover and Cherry.IsMouseClicked(1) then
        drawColor = active
    end

    Cherry.DrawRect(x, y, w, h, drawColor)

    local tx = x + (w - Cherry.CalcTextSizeX(label)) * 0.5
    local ty = y + (h - Cherry.CalcTextSizeY(label)) * 0.5
    Cherry.DrawText(tx, ty, 14, "#FFFFFF", label)

end)