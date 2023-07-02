#pragma once
#include "config.h"

void Config::loadFrom(ConfigItems &items) {
  for (auto &i : items) {
    this->set(std::get<0>(i), std::get<1>(i), std::get<2>(i));
  }
}

ConfigItems tomlConfigPauser(string_view filename) {
  auto config = toml::parse_file(filename);
  ConfigItems data;
  for (auto &&[space, v] : config) {
    if constexpr (toml::is_table<decltype(v)>) {
      for (auto &&[k, v] : *v.as_table()) {
        if (v.is<ConfigSupportedTypes>())
          data.push_back(std::make_tuple(space.str(), k.str(),
                                         *v.as<ConfigSupportedTypes>()));
      }
    }
  };
}

void Config::loadFromFile(string_view filename) {
  string_view ext = getFileExt(filename);
  //   for (auto &i : G_config_pausers) {
  //     if (i.first == ext) {
  //       auto data = i.second(filename);
  //       return this->loadFrom(data);
  //     }
  //   }
  if (ext == "toml") {
    auto data = tomlConfigPauser(filename);
    this->loadFrom(data);
  }
}

template <typename T>
T Config::get(string_view space, string_view key,
              std::function<T()> const &default_generator) {
  auto value = this->values.find(this->_generateKey(space, key));
  if (value == this->values.end()) {
    return default_generator();
  } else {
    return std::get<T>(value->second);
  }
}

template <typename T>
void Config::set(string_view space, string_view key, T value) {
  this->values.insert(std::make_pair(this->_generateKey(space, key), value));
}

template <typename T> T Config::require(string_view space, string_view key) {
  auto config_key = this->_generateKey(space, key);
  auto value = this->values.find(config_key);
  if (value == this->values.end()) {
    throw joinStr("Required config ", config_key, " not exist");
  } else {
    return std::get<T>(value->second);
  }
}
