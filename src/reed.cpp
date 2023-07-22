#include "reed.h"
// #include "frontend/frontend.h"
#include <iostream>
#include <stdlib.h>
#include <string>

Reed::Reed()
{
    Table defaultConfig, userConfig;
    try {
        defaultConfig = toml::parse_file("default_config.toml");
        userConfig = toml::parse_file("config.toml");
    } catch (const toml::parse_error &err) {
        std::cerr << err;
        exit(EXIT_FAILURE);
    }
    std::cout << config;
    for (auto &&[key, value] : defaultConfig)
    {
        if (userConfig.contains(key))
        {
            config.erase(key);
            auto insert = userConfig.find(key);
            config.insert(insert->first, insert->second);
        }
        else
        {
            config.insert(key, value);
        }
    }
}

Table &Reed::getMainWindowConfig()
{
    return config;
}