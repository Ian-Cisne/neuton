
#include "logger.hpp"

#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace neu {
    class Window {
    public:
        Window(int, int, std::string, Logger&);
        ~Window();

        bool shouldClose() { return glfwWindowShouldClose(window);}
        vk::Extent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        void createWindowSurface(vk::raii::Instance &instance, VkSurfaceKHR *surface);
        
    private:
        void initWindow();

        const int width;
        const int height;

        Logger& logger_;
        GLFWwindow * window;
        
        vk::raii::SurfaceKHR surface = nullptr;
        std::string windowName;
    };
    
}