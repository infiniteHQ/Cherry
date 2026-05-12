Cherry provides a full API to play sounds and lets you create all kinds of sound effects to enhance the user experience—for example, with navigation sounds.

<banner type="warning"> You need to compile with the compiler option `CHERRY_ENABLE_AUDIO` set to `ON`! </banner>

### Setup the Sound Engine
First, go to your CMake build file and set the CHERRY_ENABLE_AUDIO option to ON, like so:

```c
option(CHERRY_ENABLE_AUDIO "Enable audio features" ON)
```

You must then enable audio support during your application setup.

```cpp
CherryApplication *Cherry::CreateApplication(int argc, char **argv) {
  Cherry::ApplicationSpecification config;
  config.UseAudio(); // Turn on the audio engine
  return new CherryApplication(config);
}

```

And there you go, the audio module is now operational!

### Play sound

To play a sound, simply call the Cherry app and use the `PlaySound` function. You can then specify a sound file, such as a `.wav` file, like this:

```cpp
CherryApp.PlaySound("path/to/sound.wav");
```

For added simplicity, you can directly use a sound as an asset.
Here is the function:

<banner type="note"> To learn how to add and use assets, refer to the section [Paths & Assets](https://i.n:content_name=learn&page_name=path_assets&section=all) </banner>

```cpp
CherryApp.PlaySound(CherryPath("resources/audio/tick.wav"));
```

Finally, here's a small example of a button that plays a little tick sound! Very cool for user experience:

```cpp
if (CherryKit::ButtonText("Click me !").GetDataAs<bool>("isClicked")) {
  CherryApp.PlaySound(CherryPath("resources/audio/tick.wav"));
}
```