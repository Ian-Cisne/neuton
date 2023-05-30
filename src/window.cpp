
#include "window.hpp"

#include <string>
#include <iostream>
#include <format>

namespace neu {
	Window::Window(int w, int h, std::string name, Logger &logger) :
		width{w}, height{h}, windowName{name}, logger_{logger} {
		glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

	Window::~Window() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	std::vector<const char*> Window::getRequiredExtensions() {
		uint32_t count;
		const char **req = glfwGetRequiredInstanceExtensions(&count);
		return std::vector<const char*> (req, req + count);
	}
	
	vk::raii::SurfaceKHR Window::createWindowSurface(vk::raii::Instance const &instance) {
		VkSurfaceKHR surface;
		VkResult res = glfwCreateWindowSurface(static_cast<VkInstance>( *instance ), window, nullptr, &surface);
		if( res != VK_SUCCESS) {
			throw std::runtime_error(std::format("Failed to create window surface! (cod:{})\n", static_cast<int>(res)));
		}
		return  vk::raii::SurfaceKHR(instance, surface);

	}
}