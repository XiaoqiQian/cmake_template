#include "configx.h"

#include <fstream>

#include "json/json.hpp"
namespace project
{
    using json = nlohmann::json;
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(project::AppConfig::Settings, osrEnabled, devToolsEnabled, chromeRuntimeEnabled,
                                       cachePath, maxCacheSizeByte, defaultUrl, browserBackgroundColorARGB,
                                       minimumFontPixelSize)

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(project::AppConfig::LogSettings, dir, maxFileSizeMB, maxFileNum, pattern,
                                       logLevel, flushLevel)

    bool AppConfig::Parse(const char *filepath)
    {
        try
        {
            std::ifstream f(filepath);
            if (!f.is_open())
                return false;

            json root;
            f >> root;

            // 1. 解析 settings
            if (root.contains("settings"))
                root["settings"].get_to(m_settings);

            // 2. 解析 log
            if (root.contains("log"))
                root["log"].get_to(m_logSettings);

            // 3. 解析 commandLines（核心修正点！）
            if (root.contains("commandLines"))
            {
                const auto &cmd = root["commandLines"];

                // 3.1 switches 数组 → m_switchs
                if (cmd.contains("switches"))
                    cmd["switches"].get_to(m_switchs);

                // 3.2 switchWithValue 对象 → m_switchsWithValue
                if (cmd.contains("switchWithValue"))
                    cmd["switchWithValue"].get_to(m_switchsWithValue);
            }

            return true;
        }
        catch (const std::exception &)
        {
            return false;
        }
    }

    AppConfig &AppConfig::Instance()
    {
        static AppConfig config;
        return config;
    }
} // namespace project
