#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <iostream>
#include <vector>

enum class LogType {
    INFO,
    WARN,
    ERROR
};

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
    public:
        static void Log(const std::string& message);
        static void Err(const std::string& message);
        static std::vector<LogEntry> logBuff;
};

#endif
