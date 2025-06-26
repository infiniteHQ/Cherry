Cherry enables advanced usage while maintaining simplicity through graphical and logical components that you can use to build your applications.

<banner type="note">Cherry includes a comprehensive kit of ready-to-use components for your application that you can call using the `CherryKit::` namespace.</banner>

### Understand Components
As introduced with Cherry, it supports different rendering modes to suit various needs and contexts. These rendering modes directly influence how components are rendered within your application.

Cherry vous permet differentes approches lors de la creation de vos composants.

#### Identifiers
Here are the possible identifications of components available with Cherry:

| **Identifications**   | **Description**                                                                 |
|-----------------------|---------------------------------------------------------------------------------|
| **Anonymous Components** (without CherryID) | An anonymous component is not identifiable by the user or at runtime, meaning there is no way to access or manipulate it remotely after instantiation—unless it is part of a group or pool. The component remains completely anonymous. |
| **Identified Components** (with CherryID)  | A component identified using a CherryID allows the user to access it remotely (based on its ID), in order to manipulate it, retrieve or set properties, or update its data. |

###### Create Anonymous Component
```cpp
// This component is defined without a CherryID, it is anonymous.
CherryKit::TextSimple("Anonymous Text !");
```

###### Create Identified Component
```cpp
// This component is defined with a CherryID, it is identifiable.
CherryKit::TextSimple(CherryID("text"), "Anonymous Text !");

// I can use this ID to set or get data and properties
CherryApp.GetComponent(CherryID("text")).SetProperty("color_text", "#B1FF31");
```

#### Render modes
Here are the possible render modes available with Cherry:

| **Render Mode**   | **Description**                                                                 |
|-------------------|---------------------------------------------------------------------------------|
| **Immediate Mode** (Inline) | The component is rendered instantly and discarded after the current frame. Ideal for UI elements that do not require state retention. |
| **Static Mode** (CreateOnly)  | The component is rendered when the user want and its state is retained across frames. Useful for components with persistent visual or logical data. |
| **Semi-Static Mode** *Default* | The component is rendered using immediate, but its data remains static and persistent, allowing for consistent behavior across the application's lifecycle. |

###### Create Immediate Mode Component

This component will be rendered in immediate mode with the Render mode set to `RenderMode::Inline`, meaning the component will not be retained in terms of rendering or data. No component will be created in the runtime — it will be rendered, consumed, and then destroyed. No data will persist, this is true functional immediate rendering.
```cpp
CherryNextComponent.SetRenderMode(RenderMode::Inline);
CherryKit::TextSimple("Anonymous Text !");
```

###### Create Static Component

A static component is created with the render node `RenderMode::CreateOnly`, meaning the component is fully retained, both data and rendering. The component is created and stored, awaiting rendering as triggered by the user. This component has nothing to do with the concept of immediate rendering.

```cpp
CherryNextComponent.SetRenderMode(RenderMode::CreateOnly);
CherryKit::TextSimple("Anonymous Text !");

// Call the render when we want
CherryApp.GetComponent(CherryID("text")).RenderWrapper();
```

###### Create Semi-Static Component

Semi-static rendering is one of Cherry’s unique features — it is the default render node, so there is no need to explicitly set it. This rendering mode combines the best of both worlds: immediate mode and retained mode. That means rendering will be responsive (thanks to immediate rendering), and the data will remain resilient and stable (thanks to retained management).

```cpp
CherryKit::TextSimple("Anonymous Text !");
```

<banner type="note">To learn how to use these components in practice, see the section [Use Components]().</banner>

### Use Components
Now let’s explore how to use components!

<banner type="note">In this example, we will call a simple component: `TextSimple`. All standard Cherry components work in a similar way, except for complex ones (in such cases, refer to the Components Reference section of the documentation).</banner>

Let’s start with the basics: calling a Cherry component is simple, just invoke it by its name and provide the parameters it requires.

To use a component, simply call its interface. (The standard Cherry interface is `CherryKit::`).

You can create an anonymous component without using an ID.
```cpp
CherryKit::TextSimple("Anonymous Text !");
```

You can also create an identifiable component using an ID.
```cpp
// This component is defined with a CherryID, it is identifiable.
CherryKit::TextSimple(CherryID("text"), "Anonymous Text !");

// I can use this ID to set or get data and properties
CherryApp.GetComponent(CherryID("text")).SetProperty("color_text", "#B1FF31");
```


You can also define the render mode.
```cpp
CherryKit::TextSimple("Semi-static text");

CherryNextComponent.SetRenderMode(RenderMode::CreateOnly);
CherryKit::TextSimple("Static text");


CherryNextComponent.SetRenderMode(RenderMode::Inline);
CherryKit::TextSimple("Immediate mode text");


CherryNextComponent.SetRenderMode(RenderMode::None);
CherryKit::TextSimple("Other semi-static text");

```

### Dynamic variables
You also have access to two dynamic variables, `CherryNextComponent` and `CherryLastComponent`, which provide you with an interactive reference to the component.

```cpp
// Will give you an access to the next component
CherryNextComponent.SetProperty("color_text", "#B1FF31");
// Component
CherryKit::TextSimple("Other semi-static text");
// Will give you an acces to the last component
CherryLastComponent.GetProperty("color_text"); // return = #B1FF31
```

### Create Components
Although Cherry includes many built-in components ready to use, it also allows you to create or import your own. Let’s take a look at how a component is structured in code and how to define your own.

Here is the basic structure of a component:

We create the component `TextSimple` inherited from the `Component` class inside the `Components` namespace. We add the properties and data we need, then create the `Render` function which will contain the CherryGUI elements or the drawing functions to render.

Next, we create the interfaces in the `Kit` namespace to make the component accessible via `CherryKit::`. We handle whether the user specifies a CherryID or not. We use the Cherry API to manage the creation and handling of the component.

```cpp 
namespace Cherry {
namespace Components {
class TextSimple : public Component {
public:
  TextSimple(const Cherry::Identifier &id, const std::string &label)
      : Component(id) {
    // Identifier
    SetIdentifier(id);

    // Colors
    SetProperty("color_text", "theme:text_color_text");

    // Informations
    SetProperty("label", label);
  }

  void Render() override {
    CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                           GetProperty("label").c_str());

    auto parent = Cherry::GetParent();
    if (parent) {
      CherryGUI::TextColored(HexToRGBA(GetProperty("color_text")),
                             parent->GetProperty("label").c_str());
    }
  }
};
} // namespace Components
namespace Kit {
inline Component &TextSimple(const Identifier &identifier,
                             const std::string &label) {
  return CherryApp.PushComponent<Cherry::Components::TextSimple>(identifier,
                                                                 label);
}

inline Component &TextSimple(const std::string &label) {
  return Cherry::Kit::TextSimple(
      Application::GenerateUniqueID(label + "TextSimple"), label);
}
} // namespace Kit
} // namespace Cherry
```

We can now call our example component anonymously: `CherryKit::TextSimple("Hello !")` or with an identifier: `CherryKit::TextSimple(CherryID("id"), "Hello !")`.