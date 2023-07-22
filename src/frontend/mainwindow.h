#pragma once

#include "FL/Fl_Window.H"
#include "reed.h"

class MainWindow: public Fl_Window
{
public:
    MainWindow(Table& windowConfig);
private:
    Table &config;
};