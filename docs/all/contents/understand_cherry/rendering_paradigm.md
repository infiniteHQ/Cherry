As mentioned in our introduction, Cherry brings together the best of immediate-mode rendering and static rendering through components and layers.

This section introduces a few key concepts to understand how application rendering works in Cherry.

Cherry gives developers the flexibility to choose a fully static approach, a fully ImGui-based approach, or a mix of both, depending on the needs of the application.

---
### ImGui Rendering

Immediate-mode rendering is a paradigm where the rendering data is stored functionally by the user. It guarantees that all rendering data is refreshed on each frame, following a fully functional approach. This method is ideal for building highly reactive applications, especially when real-time data needs to be displayed, perfect for monitoring or dashboard-style apps!

---
### Static Rendering

Static rendering is a common and powerful concept in application rendering. It involves creating a component and reserving it to be rendered and refreshed only when needed. While it is not immediate-mode, it allows for extremely high performance while still ensuring functional behavior and responsiveness.

---
### Semi-Static Rendering

Semi-static rendering is a hybrid between ImGui-style and static rendering. The graphical part is rendered using ImGui to guarantee instant visuals and highly reactive interactions. Meanwhile, the data and functional part of the component is handled statically, ensuring both low- and high-level persistence.

<banner type="note">Semi-static rendering is one of Cherry’s unique features.</banner>

---

### Multi-Context Rendering
Cherry also introduces another unique concept in application rendering: multi-context rendering. Cherry supports applications with multiple independent contexts for Dear ImGui, SDL, and Vulkan. This opens up many possibilities, such as multi-window applications or embedding sub-applications.

Moreover, Cherry allows these contexts to communicate with each other, enabling data transfers such as user data, windows, or UI components between them.