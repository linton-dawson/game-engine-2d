#include "Logger.hpp"

std::vector<LogEntry> Logger::logBuff;

void Logger::Log(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LogType::INFO;
    logEntry.message = message;
    std::cout << "\033[32;40;1mLOG: " << logEntry.message << "\033[0m\n";
    logBuff.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LogType::ERROR;
    std::cout << "\033[31;40;1mERROR: " << message << "\033[0m\n";
}

