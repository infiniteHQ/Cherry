Cherry provides an incredible starting set of ready-to-use components such as buttons, sliders, widgets, inputs, etc.

To access the built-in "Kit", simply enable `CHERRY_ENABLE_KIT` in your CMake file.

Then, you have access to a ton of ready-to-use components and style effects.

```cpp
CherryKit:: // Access to components 

// Exemple :
CherryKit::SeparatorText("Camera");

// You can also use the component directly :
if(CherryKit::SeparatorText("Camera").GetDataAs<bool>("isClicked")) {
  // Code when the button is clicked
}
```

You can go on [`main/kit/components`](https://github.com/infiniteHQ/Cherry/tree/main/main/kit/components) to see all components definitions.
