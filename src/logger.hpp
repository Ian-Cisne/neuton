#pragma once

#include <vector>
#include <ostream>
#include <fstream>
#include <string>

namespace neu {

    template <typename T>
    concept ConvertibleToStdString = std::convertible_to<T,std::string>;

    enum LogLevel {
        error,
        warn,
        info,
        success,
        debug,
        miscellaneuos
    };
    
    class Logger {
    public:

        Logger (std::ostream&);
        Logger (std::ostream&, std::string);
        
        
        friend Logger& operator<<(Logger&, LogLevel);
        friend Logger& operator<<(Logger&, char const*);
        friend Logger& operator<<(Logger& a , std::string const t);
        friend Logger& operator<<(Logger& a , int const t);
    private:
        //Makes this class non-copyable
        Logger(const Logger&);
        Logger& operator=(const Logger&);

        LogLevel level_;
        std::ostream &stream_;
        std::ofstream logfile_;

        //We use ANSI color escapes. This wouldn't work before Windows 10 though.
        static const std::vector<std::string> prefixes_;
    };
}