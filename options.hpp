#ifndef CHERRY_CONFIG_HPP
#define CHERRY_CONFIG_HPP

// Render backend (only one !)
#define CHERRY_USE_VULKAN 1
#define CHERRY_USE_OPENGL 0 // Not Implemented yet

// Optionnal features (1 == Enabled / 0 == Disabled)
#define CHERRY_ENABLE_NET 1
#define CHERRY_ENABLE_AUDIO 1
#define CHERRY_ENABLE_CEF 0

// Optionnal optimization and features remover (1 == Removed / 0 == Used)
#define CHERRY_DISABLE_WORKSPACE_SAVES 0

#endif // CHERRY_CONFIG_HPP
