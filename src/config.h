#pragma once
#include <functional>
#include <map>
#include <utility>
#include <variant>
#include <vector>

#ifndef __std_string
#    define __std_string
#    include <string>
typedef std::string string;
typedef std::string string;
#endif

#include "path.cpp"
#include "toml.hpp"
#include "utils.cpp"

typedef std::variant<int, unsigned int, char, string, double, bool>
    ConfigSupportedTypes;
typedef std::vector<std::tuple<string, string, ConfigSupportedTypes>>
    ConfigItems;

// std::vector<std::pair<string, std::function<ConfigItems(string)>>>
//     G_config_pausers;
// TODO: Supports this

class Config
{
  protected:
    std::map<string, ConfigSupportedTypes> values;
    inline string _generateKey(string space, string key)
    {
        return joinStr(key, "@", space);
    }

  public:
    Config() : Config("") {}
    Config(string filename)
    {
        if (filename.empty())
            {
                filename = this->DEFAULT_CONFIG_LOCATION;
            }
        this->loadFromFile(filename);
    }
    const string DEFAULT_CONFIG_LOCATION = "res/default_config.toml";
    void loadFrom(ConfigItems& items);
    void loadFromFile(string filename);
    template <typename T>
    T get(
        string space,
        string key,
        std::function<T()> const& default_generator);
    void set(string space, string key, ConfigSupportedTypes value);
    template <typename T>
    T require(string space, string key);
};

Config* getConfig(string name = "global", string load_filename = "");
