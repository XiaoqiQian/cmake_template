#include "configx.h"
#include "log/logx.h"
#include <iostream>
int main()
{
    project::AppConfig &config = project::AppConfig::Instance();
    if (!config.Parse("./config.json"))
    {
        return -1;
    }
    CLOG_INFO("Info", "test log out: {}", "hello, world");
    std::cout
        << "hello, world" << std::endl;
    return 0;
}
