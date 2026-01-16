currentValue = currentValue or "0"
storedValue = storedValue or nil
currentOp = currentOp or nil
resetNext = resetNext or false

local function applyOp(a, b, op)
    a = tonumber(a)
    b = tonumber(b)
    if op == "+" then return a + b end
    if op == "-" then return a - b end
    if op == "*" then return a * b end
    if op == "/" then return b ~= 0 and a / b or 0 end
    return b
end

local function handleButton(label)
    local id = label
    if Cherry.GetComponentData(id, "isClicked") ~= "true" then return end

    Cherry.PlaySound(Cherry.GetPath("resources/audio/click.mp3"))

    if label:match("%d") then
        if resetNext or currentValue == "0" then
            currentValue = label
            resetNext = false
        else
            currentValue = currentValue .. label
        end
        return
    end

    if label == "C" then
        currentValue = "0"
        storedValue = nil
        currentOp = nil
        resetNext = false
        return
    end

    if label == "=" then
        if storedValue and currentOp then
            currentValue = tostring(applyOp(storedValue, currentValue, currentOp))
            storedValue = nil
            currentOp = nil
            resetNext = true
        end
        return
    end

    if label:match("[%+%-%*/]") then
        if storedValue and currentOp and not resetNext then
            storedValue = tostring(applyOp(storedValue, currentValue, currentOp))
        else
            storedValue = currentValue
        end
        currentOp = label
        resetNext = true
        return
    end
end

local winW, winH = Cherry.GetWindowSize()

Cherry.DrawText(40, 20, 14, "#666666FF", "CHERRY INSTRUMENTS")
Cherry.DrawText(310, 20, 14, "#666666FF", "CX-2026")

local offX, offY = 380, 17
local offW, offH = 50, 20

local btnColor = "#232323FF"
local btnShadow = "#121212FF"
local btnHighlight = "#34343480"
local textColor = "#FF4444FF"

Cherry.DrawRect(offX+2, offY+2, offW, offH, btnShadow)
Cherry.DrawRect(offX, offY, offW, offH, btnColor)
Cherry.DrawLine(offX+2, offY+2, offX + offW-2, offY+2, btnHighlight, 2)
Cherry.DrawLine(offX+2, offY+2, offX+2, offY + offH-2, btnHighlight, 2)

Cherry.DrawText(offX + offW/2, offY + offH/2 - 7, 14, textColor, "OFF", true)

if Cherry.IsMouseClickedOnPos(offX, offY, offW, offH, 0) then
    Cherry.PlaySound(Cherry.GetPath("resources/audio/click.mp3"))
    Cherry.Close()
end

Cherry.DrawLineHorizontal(160, 20, winW - 20, "#000000FF", 2)
Cherry.DrawLineHorizontal(161, 20, winW - 20, "#444444FF", 1)
Cherry.DrawCalculatorScreen(currentValue)

local grid = {
    {"7","8","9","/"},
    {"4","5","6","*"},
    {"1","2","3","-"},
    {"C","0","=","+"}
}
local sx, sy = 40, 190
local bw, bh, gap = 82, 62, 12

for r = 1, #grid do
    for c = 1, #grid[r] do
        local label = grid[r][c]
        Cherry.SetDrawCursorPos(sx + (c-1)*(bw+gap), sy + (r-1)*(bh+gap))
        Cherry.DrawCalculatorButton(label)
        handleButton(label)
    end
end

Cherry.SetComponentProperty("CalculatorScreen", "label", currentValue)
