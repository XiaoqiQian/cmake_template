#pragma once
#include <format>
#include <string>

namespace project
{
        enum class LogLevel
        {
                Debug,
                Info,
                Warn,
                Error,
                Fatal,
        };
        void CustomLog(project::LogLevel level, int line, const std::string &function, const std::string &text, const std::string &loggerId);

#define CLOG_DEBUG(loggerId, fmt, ...) \
        project::CustomLog(project::LogLevel::Debug, __LINE__, __FUNCTION__, std::format(fmt, ##__VA_ARGS__), loggerId)

#define CLOG_INFO(loggerId, fmt, ...) \
        project::CustomLog(project::LogLevel::Info, __LINE__, __FUNCTION__, std::format(fmt, ##__VA_ARGS__), loggerId)

#define CLOG_WARN(loggerId, fmt, ...) \
        project::CustomLog(project::LogLevel::Warn, __LINE__, __FUNCTION__, std::format(fmt, ##__VA_ARGS__), loggerId)

#define CLOG_ERROR(loggerId, fmt, ...) \
        project::CustomLog(project::LogLevel::Error, __LINE__, __FUNCTION__, std::format(fmt, ##__VA_ARGS__), loggerId)

#define CLOG_FATAL(loggerId, fmt, ...) \
        project::CustomLog(project::LogLevel::Fatal, __LINE__, __FUNCTION__, std::format(fmt, ##__VA_ARGS__), loggerId)
};