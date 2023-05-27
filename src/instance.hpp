#pragma once

#include "logger.hpp"
#include "window.hpp"
#include <memory>

namespace neu {
    class Instance {
    public:
        Instance(Window window, Logger&);

        ~Instance() {}
    private:
        Logger &logger_;

        vk::raii::Context context_;
        std::shared_ptr<vk::raii::Instance> instance_;

        bool checkLayers(std::vector<vk::LayerProperties>&);
        std::shared_ptr<vk::raii::Instance> setupInstance(vk::ApplicationInfo);
        const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        const std::vector<const char *> instanceExtensions = {
            VK_KHR_SURFACE_EXTENSION_NAME
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
            ,VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_IOS_MVK )
            ,VK_MVK_IOS_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_MACOS_MVK )
            ,VK_MVK_MACOS_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_MIR_KHR )
            ,VK_KHR_MIR_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_VI_NN )
            ,VK_NN_VI_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )
            ,VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_WIN32_KHR )
            ,VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_XCB_KHR )
            ,VK_KHR_XCB_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_XLIB_KHR )
            ,VK_KHR_XLIB_SURFACE_EXTENSION_NAME
#elif defined( VK_USE_PLATFORM_XLIB_XRANDR_EXT )
            ,VK_EXT_ACQUIRE_XLIB_DISPLAY_EXTENSION_NAME
#endif
#if !defined( NDEBUG )
            ,VK_EXT_DEBUG_UTILS_EXTENSION_NAME
#endif
            };
    };
}