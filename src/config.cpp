#include "config.h"

#ifndef __config
#    define __config

std::map<string, Config*> G_configs;

// std::vector<std::pair<string, std::function<ConfigItems(string)>>>
//     G_config_pausers;
// TODO: Supports this

Config* getConfig(string name, string load_filename)
{
    auto data = G_configs.find(name);
    if (data == G_configs.end())
        {
            auto config = new Config(load_filename);
            G_configs.insert(std::make_pair(name, config));
            return config;
        }
    else
        {
            return data->second;
        }
}

void Config::loadFrom(ConfigItems& items)
{
    for (auto& i : items)
        {
            this->set(std::get<0>(i), std::get<1>(i), std::get<2>(i));
        }
}

ConfigItems tomlConfigPauser(string filename)
{
    auto config = toml::parse_file(filename);
    ConfigItems data;
    for (auto&& [space, v] : config)
        {
            if constexpr (toml::is_table<decltype(v)>)
                {
                    for (auto&& [k, v] : *v.as_table())
                        {
                            if (v.is<ConfigSupportedTypes>())
                                data.push_back(std::make_tuple(
                                    view2String(space.str()),
                                    view2String(k.str()),
                                    *v.as<ConfigSupportedTypes>()));
                        }
                }
        };
}

void Config::loadFromFile(string filename)
{
    string ext = getFileExt(filename);
    //   for (auto &i : G_config_pausers) {
    //     if (i.first == ext) {
    //       auto data = i.second(filename);
    //       return this->loadFrom(data);
    //     }
    //   }
    if (ext == "toml")
        {
            auto data = tomlConfigPauser(filename);
            this->loadFrom(data);
        }
}

template <typename T>
T Config::get(
    string space, string key, std::function<T()> const& default_generator)
{
    auto value = this->values.find(this->_generateKey(space, key));
    if (value == this->values.end())
        {
            return default_generator();
        }
    else
        {
            return std::get<T>(value->second);
        }
}

void Config::set(string space, string key, ConfigSupportedTypes value)
{
    this->values.insert(std::make_pair(this->_generateKey(space, key), value));
}

template <typename T>
T Config::require(string space, string key)
{
    auto config_key = this->_generateKey(space, key);
    auto value      = this->values.find(config_key);
    if (value == this->values.end())
        {
            throw joinStr("Required config ", config_key, " not exist");
        }
    else
        {
            return std::get<T>(value->second);
        }
}

#endif