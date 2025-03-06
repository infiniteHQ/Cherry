### Description  
The `ButtonImageTextImage` component displays a very simple button with text and image (png/jpg), this button can called as an object, a immediate mode element or a anonymous object.

### Usage  
```cpp
  CherryKit::ButtonImageTextImage("Text Image Button", CherryPath("resources/imgs/image.png"), CherryPath("resources/imgs/image.png"));

```
> `ButtonImageTextImage` will show a Text Image Button with a dropdown menu.  

### Preview
In idle mode
<img src="https://static.infinite.si/cherrydocs/1.3/all/imgs/components_references/button_buttontextimagedropdown.png" alt="Cherry 1" width="350">

With menu opened
<img src="https://static.infinite.si/cherrydocs/1.3/all/imgs/components_references/button_buttontextimagedropdown2.png" alt="Cherry 1" width="350">




TODO 




### Examples 
#### Create text Image button (as object)
```cpp
  CherryKit::ButtonImageTextImage(CherryID("custom_id"), "Text Image Button", CherryPath("resources/imgs/image.png"), CherryPath("resources/imgs/image.png"));
```
> `ButtonImageTextImage` with a `CherryID` will create an object that you can interact with across different points or frames of your application.

---

#### Create a immediate mode text Image button
```cpp
  CherryKit::ButtonImageTextImage(CherryInline, "Text Image Button", CherryPath("resources/imgs/image.png"),[](){
    CherryKit::SeparatorText("Example menu 1");
    CherryKit::ButtonText("Example Button");
    CherryKit::SeparatorText("Example menu 2");
    CherryKit::TextSimple("Super example in this menu");
    // All menu items & code.
  });
```
> `ButtonImageTextImage` with a `CherryID` set to `CherryInline` will create a non-static component (not a object). The component will operate in immediate mode, meaning it will be updated every frame.

---
#### Create a anonymous mode text Image button
```cpp
  CherryKit::ButtonImageTextImage("Text Image Button", CherryPath("resources/imgs/image.png"),[](){
    CherryKit::SeparatorText("Example menu 1");
    CherryKit::ButtonText("Example Button");
    CherryKit::SeparatorText("Example menu 2");
    CherryKit::TextSimple("Super example in this menu");
    // All menu items & code.
  });
```
> `ButtonImageTextImage` will create an anonymous object. This object will behave like any other object but without an identifier, making it a mix between object mode and immediate mode. It is optimized for performance while retaining the advantages and flexibility of a regular object.

---
### Properties  
Below are all the properties you can set for `ButtonImageTextImage`.  
For more details, see **[How to add properties]()**.  
| **Property Name** | **Example Value**  | **Description**                   |
|------------------|------------------|-----------------------------------|
| `color_border`       | `"#454545B2"`      | The color of the button border. |
| `color_border_hovered`       | `"#454545B2"`      | The color of the border when hovered. |
| `color_border_pressed`       | `"#454545B2"`      | The color of the border when pressed. |
| `color_bg`       | `"#454545B2"`      | The background color of the button. |
| `color_bg_hovered`       | `"#454545B2"`      | The background color when hovered. |
| `color_bg_pressed`       | `"#454545B2"`      | The background color when pressed. |
| `color_text`       | `"#454545B2"`      | The color of the button text. |
| `color_text_hovered`       | `"#454545B2"`      | The text color when hovered. |
| `color_text_pressed`       | `"#454545B2"`      | The text color when pressed. |
| `size_x`       | `"40"`      | The width of the button. |
| `size_y`       | `"10"`      | The height of the button. |
| `padding_x`       | `"6"`      | The padding of the button over X. |
| `padding_y`       | `"6"`      | The padding of the button over Y. |
| `scale`       | `"1.0"`      | The scaling factor of the button. |
| `disabled`       | `"false"`      | Whether the button is disabled. |
| `disable_time`       | `"false"`      | Whether the button has a timed disable feature. |
| `label`       | `"Text Image Button"`      | The text displayed on the button. |
| `image_path`       | `"/path/to/.png"`      | Path to the image, ATTENTION : Require an absolute path (CherryPath(relative path) is a good approach) |
---

### Data  
Here is all the data generated (or assigned) to the component.  
For more details, see **[How to manage data]()**.  

| **Data Name** | **Example Value**  | **Description**                   |
|------------------|------------------|-----------------------------------|
| `isClicked`       | `"true"`      | Indicates if the button was clicked. |
| `isPressed`       | `"true"`      | Indicates if the button is currently pressed. |
| `isHovered`       | `"true"`      | Indicates if the button is being hovered over. |
| `isActivated`       | `"true"`      | Indicates if the button is activated. |
| `isMenuActivated`       | `"true"`      | Indicates if the dropdown menu is activated. |
| `lastClicked`       | `"Thu Mar  6 14:46:44 2025"`      | The last time the button was clicked. |
| `lastPressed`       | `"Thu Mar  6 14:46:44 2025"`      | The last time the button was pressed. |
| `lastHovered`       | `"Thu Mar  6 14:46:44 2025"`      | The last time the button was hovered over. |
| `lastActivated`       | `"Thu Mar  6 14:46:44 2025"`      | The last time the button was activated. |

---


💡 *This documentation provides a structured overview of `ButtonImageTextImage`, making it easy to understand and use efficiently.*  
