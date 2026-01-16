local t = os.clock() * 64

local r = math.floor((math.sin(t) * 127) + 128)
local g = math.floor((math.sin(t + 2) * 127) + 128)
local b = math.floor((math.sin(t + 4) * 127) + 128)

local color = string.format("#%02X%02X%02X", r, g, b)

function DrawLines(color)
    for i = 0, 599 do
        Cherry.DrawRect(0, i * 3, 2000, 1, color)
    end
end

DrawLines("#343434")
Cherry.DrawRect(3, 4, 100, 200, color)