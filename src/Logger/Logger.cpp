#include "Logger.hpp"

std::vector<LogEntry> Logger::logBuff;

void Logger::Log(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LogType::INFO;
    logEntry.message = message;
    spdlog::info(logEntry.message);
    logBuff.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LogType::ERROR;
    spdlog::error(logEntry.message);
}

