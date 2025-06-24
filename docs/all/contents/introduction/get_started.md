### Install Cherry
To start developing with Cherry, you can directly download a copy of cherry and work inside, or add it into your project as a dependecie.

##### Download Cherry

You can download cherry by clone the repository. This is how you can do.
``` bash
git clone https://github.com/infiniteHQ/Cherry
```

Then, go on the Cherry folder.
``` bash
cd Cherry
```

Once downloaded, you can now enter the folder, switch to the latest release using `git checkout`, and add Cherry’s automatic dependencies via `git submodule`.
``` bash
git checkout 1.3
``` 
``` bash
git submodule update --init --recursive
```

Once these steps are complete, congratulations! Cherry is up and running.

<banner type="note">You can also download Cherry from the [GitHub releases](https://github.com/infiniteHQ/Cherry/releases)</banner>

### Dependancies
Before continuing, it is important to ensure that your build environment is valid and operational. Cherry requires practically no external dependencies, but here is some information about the environment you should have.

###### Development
The only dependency you will need is the `Vulkan SDK`. Please refer to the [LunarG website](https://www.lunarg.com/vulkan-sdk/) to install it. On Linux, you can install it directly from the [Vulkan website](https://www.lunarg.com/vulkan-sdk/), or simply add the `vulkan` and `vulkan-headers` packages if your distribution provides them.

###### Compilation
You also need a C/C++ compiler with CMake. On Microsoft Windows, simply install Visual Studio with CMake and the Desktop Development with C++ workload. On Linux, you will need CMake and a C++ compilers. Please refer to your Linux distribution’s documentation.

### Linux compilation
To verify that everything works correctly, go to the Cherry folder you previously downloaded and let's try to compile the hello world:

```bash
cd examples/hello
```

```bash
bash build.sh
```

The build will then start; check for any errors. If everything goes well, try running `build/bin/hello`, which should open a simple window displaying "Hello World". If you see this window, congratulations! You are now ready to develop in the Cherry environment!

You can directly see how to [create your first Cherry application](https://i.n:content_name=introduction&page_name=create_first_app&section=all).

### Windows compilation
To verify that everything works correctly, go to the Cherry folder you previously downloaded and let's try to compile the hello world:

```bash
cd examples\hello
```
```bash
build.bat
```

The build will then start; check for any errors. If everything goes well, try running `build\bin\hello.exe`, which should open a simple window displaying "Hello World". If you see this window, congratulations! You are now ready to develop in the Cherry environment!

You can directly see how to [create your first Cherry application](https://i.n:content_name=introduction&page_name=create_first_app&section=all).

### Troubleshooting
If the compilation fails, first make sure your system has a working C++ compiler installed.

Next, verify that you have obtained a functional copy of Vortex. It is essential to use a Release version by running `git checkout 1.3` to get the latest released version of Cherry.

If issues persist, check that the Vulkan SDK is properly installed, Vulkan is available on your system, and the Vulkan environment variables are correctly set.

If you encounter a compilation problem without having changed the build settings, please report it to us on our Discord (our forum is currently under construction).