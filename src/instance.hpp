#pragma once

#include "logger.hpp"
#include "window.hpp"
#include <memory>

namespace neu {
    
    class Instance {
    public:
        Instance(Window&, Logger&);
        ~Instance();
        
        friend VKAPI_ATTR VkBool32 VKAPI_CALL 
        debugUtilsMessengerCallback( VkDebugUtilsMessageSeverityFlagBitsEXT       messageSeverity,
                                    VkDebugUtilsMessageTypeFlagsEXT              messageTypes,
                                    VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
                                    void * pUserData );
    private:
        Logger &logger_;
        Window &window_;
        
        //DO NOT CHANGE OBJECTS ORDER
        vk::raii::Context context_;
        vk::raii::Instance instance_ = nullptr;
        vk::raii::PhysicalDevice physicalDevice_ = nullptr;
        vk::raii::SurfaceKHR surface_ = nullptr;
        vk::raii::Device device_ = nullptr;

        bool checkLayers();
        bool checkExtensions();
        void setupInstance();
        void RegisterDebugUtilsMessenger();
        void pickPhysicalDevice();
        void createDevice();
        
        std::vector<const char *> validationLayers = {
            "VK_LAYER_KHRONOS_validation"
            };
        std::vector<const char *> deviceExtensions = {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
        std::vector<const char *> instanceExtensions = {
            VK_KHR_SURFACE_EXTENSION_NAME
#if defined( VK_USE_PLATFORM_ANDROID_KHR )
            ,VK_KHR_ANDROID_SURFACE_EXTENSION_NAME
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