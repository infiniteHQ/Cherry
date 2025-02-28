### Description  
The `TitleOne` component displays a large H1 title with a clean and readable design. It is intended for use as the main heading of a page or a key section.

### Usage  
```cpp
CherryKit::TitleOne("Big Super Title");
```
> `TitleOne` will show a big H1 title.  

### Preview
<img src="https://static.infinite.si/cherrydocs/1.3/all/imgs/components_references/buttons_button_h1.png" alt="Cherry 1" width="350">

### Examples 
#### Create component title
```cpp
CherryKit::TitleOne(CherryID("custom_title_id"), "Big Super Title");
```
> `TitleOne` with a `CherryID` will create an object that you can interact with across different points or frames of your application.

---

#### Create a immediate mode title
```cpp
CherryKit::TitleOne(CherryID(CherryInline), "H1 Title");
```
> `TitleOne` with a `CherryID` set to `CherryInline` will create a non-static component. The component will operate in immediate mode, meaning it will be updated every frame.

---

### Properties  
Below are all the properties you can set for `TitleOne`.  
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
| no Data |

---


💡 *This documentation provides a structured overview of `TitleOne`, making it easy to understand and use efficiently.*  
