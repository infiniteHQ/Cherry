local value = Cherry.GetCurrentComponentProperty("label") or "0"
local str = tostring(value)

local x, y = 40, 50
local w, h = 360, 100

Cherry.DrawRectOutlineRounded(x - 6, y - 6, w + 12, h + 12, 4, "#000000FF", 3)
Cherry.DrawRectOutlineRounded(x - 5, y - 5, w + 10, h + 10, 4, "#444444FF", 1)
Cherry.DrawRectGradient(x, y, w, h, "#1F2D1FBB", "#12261255", true)
Cherry.DrawRectGradient(x, y, w, h * 0.45, "#FFFFFF15", "#FFFFFF00", true)
Cherry.DrawLine(x + 5, y + 2, x + w - 10, y + 2, "#FFFFFF1A", 1)

local segments = {
    ["0"] = {1,1,1,1,1,1,0},
    ["1"] = {0,1,1,0,0,0,0},
    ["2"] = {1,1,0,1,1,0,1},
    ["3"] = {1,1,1,1,0,0,1},
    ["4"] = {0,1,1,0,0,1,1},
    ["5"] = {1,0,1,1,0,1,1},
    ["6"] = {1,0,1,1,1,1,1},
    ["7"] = {1,1,1,0,0,0,0},
    ["8"] = {1,1,1,1,1,1,1},
    ["9"] = {1,1,1,1,0,1,1},
    ["-"] = {0,0,0,0,0,0,1}
}

local function drawSharpSegment(sx, sy, sw, sh, isVertical, color, hasGlow)
    sx = sx + 8;
    sy = sy - 5;
    if color == "transparent" then return end
    
    if hasGlow then
        local glowCol = "#4CFFB033"
        if isVertical then
            Cherry.DrawRect(sx - 2, sy, sw + 4, sh, glowCol)
        else
            Cherry.DrawRect(sx, sy - 2, sw, sh + 4, glowCol)
        end
    end

    local tip = isVertical and (sw / 2) or (sh / 2)

    if isVertical then
        Cherry.DrawRect(sx, sy + tip, sw, sh - (tip * 2), color)
        Cherry.DrawTriangle(sx, sy + tip, sx + sw, sy + tip, sx + (sw / 2), sy, color)
        Cherry.DrawTriangle(sx, sy + sh - tip, sx + sw, sy + sh - tip, sx + (sw / 2), sy + sh, color)
    else
        Cherry.DrawRect(sx + tip, sy, sw - (tip * 2), sh, color)
        Cherry.DrawTriangle(sx + tip, sy, sx + tip, sy + sh, sx, sy + (sh / 2), color)
        Cherry.DrawTriangle(sx + sw - tip, sy, sx + sw - tip, sy + sh, sx + sw, sy + (sh / 2), color)
    end
end

local function drawDigit(dx, dy, digit, isGhost)
    local seg = segments[digit] or segments["0"]
    local activeCol = "#4CFFB0FF"
    local ghostCol  = "#253325FF"
    
    local function getColor(id)
        if isGhost then return ghostCol end
        return seg[id] == 1 and activeCol or "transparent"
    end

    local thickness = 6
    local length = 35
    local spacing = 2

    drawSharpSegment(dx + thickness, dy, length, thickness, false, getColor(1), not isGhost and seg[1] == 1)
    drawSharpSegment(dx + thickness + length + spacing, dy + thickness, thickness, length, true, getColor(2), not isGhost and seg[2] == 1)
    drawSharpSegment(dx + thickness + length + spacing, dy + (thickness * 2) + length + spacing, thickness, length, true, getColor(3), not isGhost and seg[3] == 1)
    drawSharpSegment(dx + thickness, dy + (thickness * 2) + (length * 2) + (spacing * 2), length, thickness, false, getColor(4), not isGhost and seg[4] == 1)
    drawSharpSegment(dx, dy + (thickness * 2) + length + spacing, thickness, length, true, getColor(5), not isGhost and seg[5] == 1)
    drawSharpSegment(dx, dy + thickness, thickness, length, true, getColor(6), not isGhost and seg[6] == 1)
    drawSharpSegment(dx + thickness, dy + thickness + length + spacing, length, thickness, false, getColor(7), not isGhost and seg[7] == 1)
end

local maxDigits = 6
local startX = x + w - (maxDigits * 58) - 10

for i = 0, maxDigits - 1 do
    drawDigit(startX + (i * 58), y + 12, "8", true)
end

local displayStr = str:sub(-maxDigits)
local offset = maxDigits - #displayStr
for i = 1, #displayStr do
    local char = displayStr:sub(i,i)
    if segments[char] or char == "0" then
        drawDigit(startX + ((i + offset - 1) * 58), y + 12, char, false)
    end
end