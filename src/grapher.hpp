#pragma once
#include "instance.hpp"
#include "logger.hpp"
#include "utils.hpp"

namespace neu {
    class Grapher {
    public:
        Grapher() = delete;
        Grapher(Logger&);
        void run();
    private:
        static constexpr int width = 800;
        static constexpr int height = 600;
        Logger &logger_;
        Window window_{width, height, "Grapher", logger_};
        Instance instance_{window_, logger_};
    };
}