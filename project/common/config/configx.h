#pragma once
#include <map>
#include <string>
#include <vector>
namespace project
{

    class AppConfig
    {
    public:
        struct Settings
        {
            bool osrEnabled{false};                                          ///< 是否启用OSR
            bool devToolsEnabled{false};                                     ///< 是否启用开发者工具
            bool chromeRuntimeEnabled{false};                                ///< 是否启用ChromeRuntime
            std::string cachePath{".cefCache/"};                             ///< 网页缓存路径
            size_t maxCacheSizeByte{0};                                      ///< 缓存目录最大占用，超过则清理，单位字节。值为0时表示不持久化会话Cookie
            std::string defaultUrl{"about:blank"};                           ///< 默认URL
            std::vector<int> browserBackgroundColorARGB{255, 255, 255, 255}; ///< 浏览器背景颜色
            int32_t minimumFontPixelSize{6};                                 ///< 网页支持的最小字体大小
        };

        struct LogSettings
        {
            std::string dir;          ///< 日志文件存放路径
            size_t maxFileSizeMB{10}; ///< 单个日志文件最大体积，单位MB
            size_t maxFileNum{5};     ///< 同一个loggerId下最大日志文件数量
            std::string pattern;      ///< 日志格式
            int32_t logLevel{2};      ///< 最小打印级别
            int32_t flushLevel{2};    ///< 会触发刷新的日志级别
        };
        static AppConfig &Instance();

        Settings GetSettings() const
        {
            return m_settings;
        }

        LogSettings GetLogSettings() const
        {
            return m_logSettings;
        }

        std::vector<std::string> GetSwitchs() const
        {
            return m_switchs;
        }

        std::map<std::string, std::string> GetSwitchsWithValue()
        {
            return m_switchsWithValue;
        }
        bool Parse(const char *filepath);

    private:
        AppConfig() = default;

    private:
        std::vector<std::string> m_switchs;
        std::map<std::string, std::string> m_switchsWithValue;
        Settings m_settings;
        LogSettings m_logSettings;
    };

}; // namespace project
