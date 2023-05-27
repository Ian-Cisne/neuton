
#include "window.hpp"

#include <string>

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
  void Window::createWindowSurface(vk::raii::Instance &instance, VkSurfaceKHR *surface) {
    if(glfwCreateWindowSurface(*instance, window, nullptr, surface) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create window surface!\n");
    }
  }
}