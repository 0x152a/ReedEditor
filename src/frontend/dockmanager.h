#pragma once

#include <FL/Fl_Widget.H>
#include <FL/Fl_Tile.H>

class MainWindow;

//Fl_Wizard Fl_Tile
class DockManager : Fl_Tile
{
public:
    DockManager(MainWindow *parent);
private:
    MainWindow *parent;
};