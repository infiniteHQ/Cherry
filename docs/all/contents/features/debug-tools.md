<banner type="warning"> This feature is still in active development, bugs can occur</banner>

If enabled, Cherry embeds native debug tools, including a component inspector, a console, and some debug utilities (similar to the DevTools of some web browsers).

To enable debug tools, simply turn `CHERRY_DEBUG` to `ON` on your cmake, then you have access to the `CherryApp.ToggleDebugTools()` to show/hide devtools.
You can also trigger devtools with `F12` key like a web browser.

This a view of devtools :

<img width="800" height="450" alt="devtools" src="https://static.infinite.si/cherrydocs/1.6/all/media/devtools.gif" />
