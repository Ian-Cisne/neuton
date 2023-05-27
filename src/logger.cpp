 #include "logger.hpp"

 #include <chrono>

 namespace neu {

    Logger::Logger(std::ostream &stream): stream_{stream} {}
    Logger::Logger(std::ostream &stream, std::string outPath): stream_{stream}, logfile_{outPath, std::ofstream::ate} {}
    //ANSI color escapes
    const std::vector<std::string> Logger::prefixes_ (
        {
            "\033[31;1m", //error
            "\033[33;1m", //warn
            "\033[34;1m", //info
            "\033[92;3m",  //debug
            "\033[01;3m"  //miscellaneuos
        }
    );
    
    Logger& operator<<(Logger& a, LogLevel level) {
        a.level_ = level;
        return a;
    }

    template<ConvertibleToStdString T> 
    Logger& operator<<(Logger& a , T t) {
        a.stream_ << std::format("[{:%T}]: ",std::chrono::system_clock::now())
            + a.prefixes_[a.level_] + t + "\033[0m" << std::endl;

        if (a.logfile_.is_open()) {
            a.logfile_ <<  std::format("[{:%T}]: ",std::chrono::system_clock::now()) + t << std::endl;
        }
        return a;
    }
    
    Logger& operator<<(Logger& a , char const* t) {
        a.stream_ << std::format("[{:%T}]: ",std::chrono::system_clock::now()) 
            + a.prefixes_[a.level_] + t + "\033[0m" << std::endl;

        if (a.logfile_.is_open()) {
            a.logfile_ << std::format("[{:%T}]: ",std::chrono::system_clock::now()) + t << std::endl;
        }
        return a;
    }


 }