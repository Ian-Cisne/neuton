#include "utils.hpp"
#include "logger.hpp"


#include <chrono>
#include <format>
namespace neu {

    Logger::Logger(std::ostream &stream): stream_{stream} {}
    Logger::Logger(std::ostream &stream, std::string outPath): stream_{stream}, logfile_{outPath, std::ofstream::ate} {}

    Logger::~Logger() {
        stream_ <<  "\033[0m";
    }

    //ANSI color escapes
    const std::vector<std::string> Logger::prefixes_ (
        {
            "\033[31;1m", //error
            "\033[33;1m", //warn
            "\033[34;1m", //info
            "\033[32;1m", //success
            "\033[92;3m",  //debug
            "\033[01;3m"  //miscellaneuos
        }
    );
    
    Logger& operator<<(Logger& a, LogLevel level) {
#ifdef NEU_LOG
        a.level_ = level;
        a.stream_ << a.prefixes_[level] << std::format("[{:%T}]: ",std::chrono::system_clock::now()) ;

        if (a.logfile_.is_open()) {
            a.logfile_ << std::format("[{:%T}]: ",std::chrono::system_clock::now());
        }
#endif
        return a;
    }
    
    Logger& operator<<(Logger& a , char const* t) {
#ifdef NEU_LOG
        a.stream_ << t;;

        if (a.logfile_.is_open()) {
            a.logfile_ << t;
        }
#endif
        return a;
    }
    
    Logger& operator<<(Logger& a , std::string const t) {
#ifdef NEU_LOG
        a.stream_ << t;

        if (a.logfile_.is_open()) {
            a.logfile_ << t;
        }
#endif
        return a;
    }

    Logger& operator<<(Logger& a , int const t) {
#ifdef NEU_LOG
        a.stream_ << t;

        if (a.logfile_.is_open()) {
            a.logfile_ << t;
        }
#endif
        return a;
    }


 }