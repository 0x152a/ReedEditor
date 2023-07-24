#pragma once

#include "reed.h"

#include <FL/Fl_Double_Window.H>
#include <FL/Enumerations.H>

#include <vector>

class DockManager;

class MainWindow: public Fl_Double_Window
{
public:
    MainWindow(Table& Config);
private:
    Table &config;
    DockManager *dockManager;
};