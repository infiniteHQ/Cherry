# ButtonText  

### Description  
The `ButtonText` component is used to display a **simple button with text** on the screen.  

### Usage  
```cpp
CherryKit::ButtonText("Super button");
```
```cpp
// To create ID component
CherryKit::ButtonText(CherryID("custom_title_id"), "Super button");
```
> `ButtonText` will show a simple button with text.

### Properties  
Below are all the properties you can set for `ButtonText`.  
For more details, see **[How to add properties]()**.  

| **Property Name** | **Example Value**                 | **Description**                           |
|------------------|---------------------------------|-------------------------------------------|
| `label`          | `"My Button"`                     | The text displayed in the button.         |
| `color_border`          | `"#FF1111FF"`                     | The color of the borders         |
| `color_border_hovered`          | `"#FF1111FF"`                     | The color of the borders when the button is hovered         |
| `color_border_clicked`          | `"#FF1111FF"`                     | The color of the borders when the button is clicked       |
| `color_bg`          | `"#FF1111FF"`                     | The color of the background         |
| `color_bg_hovered`          | `"#FF1111FF"`                     | The color of the background when the button is hovered         |
| `color_bg_clicked`          | `"#FF1111FF"`                     | The color of the background when the button is clicked         |
| `size_x`          | `"16.0"`                     | The size of the button over the X axis *(horizontal size)*         |
| `size_y`          | `"6.0"`                     | The size of the button over the Y axis *(vertical size)*         |  |

---

### Data  
Here is all the data generated (or assigned) to the component.  
For more details, see **[How to manage data]()**.  

| **Property Name** | **Example Value**  | **Description**                   |
|------------------|------------------|-----------------------------------|
| `hovered`       | `"true"`      | The boolean data when the button is hovered. |
| `lastClicked`       | `""`      | The last time the button was clicked. |
| `isClicked`       | `"true"`      | Is the button is currently clicked ? (pressed + released)
| `isPressed`       | `"true"`      | Is the button is currently clicked ? (pressed)
---

### More advanced usage  
```cpp
Cherry::SetNextComponentProperty("color_border_hovered", "FF1111FF");
Cherry::SetNextComponentProperty("color_border", "FF1111FF");
Cherry::SetNextComponentProperty("color_bg_hovered", "FF1111FF");
Cherry::SetNextComponentProperty("color_bg", "FF1111FF");
CherryKit::ButtonText("Super button");
```
> `ButtonText` will show a simple button with text.

IMAGE

💡 *This documentation provides a structured overview of `ButtonText`, making it easy to understand and use efficiently.*  
