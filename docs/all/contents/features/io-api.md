Cherry embeds an Inputs/Outputs API to quickly get user actions, key presses, mouse actions, etc.

```cpp
  if (CherryApp.IsKeyPressed(Cherry::CherryKey::Z)) {
    // code here
  } 
  
  if (CherryApp.IsKeyPressed(Cherry::CherryKey::CTRL)) {
    // code here
  }
  
  if (CherryApp.IsMouseClicked(0)) {
    // left click
  }

  if (CherryApp.IsMousePressed(0)) {
    // left mouse button pressed
  }
```

### Get input key

You can simply check if a key is pressed.

```cpp
if(CherryApp.IsKeyPressed(Cherry::CherryKey::A))
{
    // Key pressed !
}
```