#include "reed.h"
#include "frontend/frontend.h"
#include <iostream>
#include <stdlib.h>
#include <string>

Reed::Reed()
{
    Table defaultConfig, userConfig;
    try {
        defaultConfig = toml::parse_file("default_config.toml");
        
    } catch (const toml::parse_error& err) {
        std::cerr << err;
        exit(EXIT_FAILURE);
    }
    try {
        userConfig = toml::parse_file("config.toml");
    } catch (const toml::parse_error& err) {
        std::cerr << err;
        exit(EXIT_FAILURE);
    }
    
    this->mainWindowConfig = *defaultConfig["main-window"].as_table();
}

Table& Reed::getMainWindowConfig()
{
    return mainWindowConfig;
}