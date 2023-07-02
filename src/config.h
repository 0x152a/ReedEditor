#include <functional>
#include <map>
#include <utility>
#include <variant>
#include <vector>

#ifndef __std_string
#    define __std_string
#    include <string>
typedef std::string string;
typedef std::string_view string_view;
#endif

#include "path.cpp"
#include "toml.cpp"
#include "utils.cpp"

typedef std::variant<int, unsigned int, char, string_view, double, bool>
    ConfigSupportedTypes;
typedef std::vector<std::tuple<string_view, string_view, ConfigSupportedTypes>>
    ConfigItems;

// std::vector<std::pair<string_view, std::function<ConfigItems(string_view)>>>
//     G_config_pausers;
// TODO: Supports this

class Config
{
  protected:
    std::map<string_view, ConfigSupportedTypes> values;
    inline string_view _generateKey(string_view space, string_view key)
    {
        return joinStr(key, "@", space);
    }

  public:
    Config() : Config(DEFAULT_CONFIG_LOCATION) {}
    Config(string_view filename)
    {
        this->loadFromFile(filename);
    }
    const string_view DEFAULT_CONFIG_LOCATION = "res/default_config.toml";
    void loadFrom(ConfigItems& items);
    void loadFromFile(string_view filename);
    template <typename T>
    T get(
        string_view space,
        string_view key,
        std::function<T()> const& default_generator);
    template <typename T>
    void set(string_view space, string_view key, T value);
    template <typename T>
    T require(string_view space, string_view key);
};