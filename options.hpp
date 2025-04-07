#ifndef CHERRY_CONFIG_HPP
#define CHERRY_CONFIG_HPP

// Render backend (only one !)
#define CHERRY_USE_VULKAN
//#define CHERRY_USE_OPENGL // Not Implemented yet

// Optionnal features (1 == Enabled / 0 == Disabled)
//#define CHERRY_ENABLE_NET
//#define CHERRY_ENABLE_AUDIO
//#define CHERRY_ENABLE_WEBVIEW
#define CHERRY_ENABLE_CEF
#define CHERRY_CEF


// Optionnal optimization and features remover (1 == Removed / 0 == Used)
//#define CHERRY_DISABLE_WORKSPACE_SAVES

#endif // CHERRY_CONFIG_HPP
