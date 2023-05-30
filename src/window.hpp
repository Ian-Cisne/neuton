#pragma once
#include "logger.hpp"

#include "utils.hpp"

namespace neu {
    class Window {
    public:
        Window(int, int, std::string, Logger&);
        ~Window();

        bool shouldClose() { return glfwWindowShouldClose(window);}
        bool getKey(glfwKey key) {return glfwGetKey(window, key); } 
        vk::Extent2D getExtent() { return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }
        vk::raii::SurfaceKHR createWindowSurface(vk::raii::Instance const &instance);
        Window(const neu::Window&) = delete;
        std::vector<const char*> getRequiredExtensions();
        
    private:
        void initWindow();

        const int width;
        const int height;

        Logger& logger_;
        GLFWwindow * window;
        
        std::string windowName;
    };
    
}