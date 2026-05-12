
<banner type="note"> This is an advanced use of Cherry, you need to be comfortable with GLSL, Cherry, Dear ImGui, Vulkan and core shader concepts.</banner> 

Cherry supports compute shaders. A compute shader is not necessarily for rendering itself but more for side calculations you can give to the GPU. You can for example calculate custom textures, calculate Perlin noise, or apply complex mathematical problems.

This is an example from the [noises compute shader example](https://github.com/infiniteHQ/Cherry/tree/main/examples/concepts/compute_shaders_noise) :

<img width="800" height="450" alt="noise_cs" src="https://static.infinite.si/cherrydocs/1.6/all/media/computeshader.gif" />

This is another example from the [textures compute shader example](https://github.com/infiniteHQ/Cherry/tree/main/examples/concepts/compute_shaders) :

<img width="800" height="450" alt="textures_cs" src="https://github.com/user-attachments/assets/874c0769-0d56-4976-b808-e13d19c5b64d" />
