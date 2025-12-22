### Description  
The `TitleFive` component displays a large Small Title with a clean and readable design. It is intended for use as the main heading of a page or a key section.

### Usage  
```cpp
CherryKit::TitleFive("Small Title");
```
> `TitleFive` will show a Small Title.  

### Preview
<img src="https://static.infinite.si/cherrydocs/1.4/all/imgs/components_references/buttons_button_h5.png" alt="Cherry 1" width="350">

### Examples 
#### Create title (as object)
```cpp
CherryKit::TitleFive(CherryID("custom_id"), "Small Title");
```
> `TitleFive` with a `CherryID` will create an object that you can interact with across different points or frames of your application.

---

#### Create a immediate mode title
```cpp
CherryKit::TitleFive(CherryInline, "Small Title");
```
> `TitleFive` with a `CherryID` set to `CherryInline` will create a non-static component (not a object). The component will operate in immediate mode, meaning it will be updated every frame.

---
#### Create a anonymous mode title
```cpp
CherryKit::TitleFive("Small Title");
```
> `TitleFive` will create an anonymous object. This object will behave like any other object but without an identifier, making it a mix between object mode and immediate mode. It is optimized for performance while retaining the advantages and flexibility of a regular object.

---
### Properties  
Below are all the properties you can set for `TitleFive`.  
For more details, see **[How to add properties]()**.  
| **Property Name** | **Example Value**  | **Description**                   |
|------------------|------------------|-----------------------------------|
| `color_text`       | `"#454545B2"`      | The color of the title. |
| `label`       | `"My Title"`      | The text of the title. |

---

### Data  
Here is all the data generated (or assigned) to the component.  
For more details, see **[How to manage data]()**.  

| **Data Name** | **Example Value**  | **Description**                   |
|------------------|------------------|-----------------------------------|
| no data |

---


💡 *This documentation provides a structured overview of `TitleFive`, making it easy to understand and use efficiently.*  
