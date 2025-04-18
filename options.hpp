#pragma once

//──────────────────────────────────────────────────────────
// Render API (only one !)
//──────────────────────────────────────────────────────────
  #define CHERRY_USE_VULKAN     // Use Vulkan as renderer
//#define CHERRY_USE_OPENGL     // Use OpenGL as renderer // WARNING : Not Implemented yet
//──────────────────────────────────────────────────────────




//──────────────────────────────────────────────────────────
// Optionnal features (1 == Enabled / 0 == Disabled)
//──────────────────────────────────────────────────────────
  #define CHERRY_ENABLE_NET     // Enable all net code with restcpp (http images)
//#define CHERRY_ENABLE_AUDIO   // Enable audio engine and allow to play mp3 & wav sounds effects / musics
//#define CHERRY_ENABLE_WEBVIEW // Enable webviews with CEF (Chrome Embedded Framework)
//#define CHERRY_ENABLE_CEF     // OBSOLETE
//#define CHERRY_CEF            // OBSOLETE
//──────────────────────────────────────────────────────────

#define IMGUI_ENABLE_FREETYPE


//──────────────────────────────────────────────────────────
// Removable features (1 == Removed / 0 == Used)
//──────────────────────────────────────────────────────────
//#define CHERRY_DISABLE_NODES              // Disable nodal system (blueprint) // WARNING : Not Implemented yet
//#define CHERRY_DISABLE_NOTIFICATIONS      // Disable notification system // WARNING : Not Implemented yet
//#define CHERRY_DISABLE_WORKSPACE_SAVES    // Disable workspace recording (to save states and window status) // WARNING : Not Implemented yet
//──────────────────────────────────────────────────────────