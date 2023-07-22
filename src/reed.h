#pragma once

/*
前后端稍微解一下耦，各种核心逻辑全放到这里处理
前端事件也传过来处理
*/

#include <variant>
#include <toml.hpp>
using std::get;

typedef toml::table Table;

class Frontend;

class Reed
{
  public:
    Reed();
    Table& getMainWindowConfig();
  private:
    Table data;
    Table mainWindowConfig;
    
    Frontend *frontend;
    
};