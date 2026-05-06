local i = 0
Cherry.CreateHook("five_second_trigger", "second",
    function()
        i = i + 1;
        if i >= 5 then
            i = 0;
            Cherry.SetHookData("five_second_trigger", "active", "true")
        else
            Cherry.SetHookData("five_second_trigger", "active", "false")
        end
    end
)