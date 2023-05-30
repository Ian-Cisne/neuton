
#include "grapher.hpp"
#include <chrono>
#include <thread>
namespace neu {
    Grapher::Grapher(Logger& logger): logger_{logger} {

    }
    void Grapher::run() {
        while (!window_.shouldClose() && window_.getKey(glfwKey::kEscape) != GLFW_PRESS) {    
            glfwPollEvents();
        }
    }
}