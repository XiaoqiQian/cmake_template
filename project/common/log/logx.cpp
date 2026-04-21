#include "logx.h"
#include "configx.h"

#include "sinks/rotating_file_sink.h"
#include "spdlog.h"

namespace project
{
    std::shared_ptr<spdlog::logger> GetLoggerById(const std::string &loggerId)
    {
        static std::map<std::string, std::shared_ptr<spdlog::logger>> loggerMap;
        static std::mutex mtx;
        std::lock_guard<std::mutex> lck(mtx);
        if (loggerMap.count(loggerId))
        {
            return loggerMap[loggerId];
        }
        try
        {
            AppConfig& config = AppConfig::Instance();
            AppConfig::LogSettings settings = config.GetLogSettings();
            std::string logFilePath;
            if (settings.dir.empty())
            {
            }
            else
            {
                logFilePath = settings.dir + loggerId + ".log";
            }
            auto logger = spdlog::rotating_logger_mt(
                loggerId,
                logFilePath,
                settings.maxFileSizeMB,
                settings.maxFileNum);

            // logger->set_pattern(g_logConfig.pattern);
            loggerMap[loggerId] = logger;
            return logger;
        }
        catch (const spdlog::spdlog_ex &ex)
        {
            // 捕获 spdlog 异常，防止程序崩溃
            spdlog::error("日志初始化失败: {}，错误信息: {}", loggerId, ex.what());
            return nullptr;
        }
    }

    void CustomLog(project::LogLevel level, int line, const std::string &function, const std::string &text,
                   const std::string &loggerId)
    {
        auto logger = GetLoggerById(loggerId);
        if (!logger)
        {
            // 如果logger为nullptr，使用spdlog的默认logger输出错误信息
            spdlog::error("Logger not initialized for id: {}", loggerId);
            return;
        }
        switch (level)
        {
        case project::LogLevel::Debug:
            logger->debug("[{}@{}]{}", function, line, text);
            break;
        case project::LogLevel::Info:
            logger->info("[{}@{}]{}", function, line, text);
            break;
        case project::LogLevel::Warn:
            logger->warn("[{}@{}]{}", function, line, text);
            break;
        case project::LogLevel::Error:
            logger->error("[{}@{}]{}", function, line, text);
            break;
        case project::LogLevel::Fatal:
            logger->critical("[{}@{}]{}", function, line, text);
            break;
        default:
            logger->warn("[UNKNOW_LEVEL][{}@{}] {}", function, line, text);
            spdlog::error("Unsupported LogLevel value: {}", static_cast<int>(level));
            break;
        }
    }
}; // namespace project