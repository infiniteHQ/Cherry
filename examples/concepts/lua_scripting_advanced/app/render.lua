Cherry.Draw("Button", {
    ["id"]          = "btnId",
    ["prop:y"]      = 24,
    ["prop:width"]  = "120",
    ["prop:height"] = "20",
    ["prop:label"]  = "Click now !!!"
})

local isClicked = Cherry.GetComponentData("btnId", "isClicked")

if isClicked == "true" then
    print("clicked !!")
end

local active  = Cherry.GetHookData("five_second_trigger",  "active") or "false"
if active == "true" then
    print("5 seconds trigger !")

    -- Reset remotely
    Cherry.GetHookData("five_second_trigger",  "active", "false")
end

Cherry.SetDrawCursorPosY(Cherry.GetDrawCursorPosY() + 40)

-- Can call components created in C++
Cherry.Draw("ButtonText", {
    ["id"]          = "btnId2",
    ["prop:label"]  = "Click super !!!"
})

Cherry.Draw("ButtonImage", {
    ["id"]          = "btnId3",
    ["prop:image_path"]  = Cherry.GetPath("resources/images/settings.png")
})