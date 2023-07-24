#include "mainwindow.h"
#include "dockmanager.h"

MainWindow::MainWindow(Table &config)
    : Fl_Double_Window(
        config["main-window"]["width"].value_or(400),
        config["main-window"]["height"].value_or(400),
        config["main-window"]["title"].value_or("Reed Editor")
    ),
    config(config)
{
    this->box(FL_NO_BOX);
    this->resizable(this);
    this->dockManager = new DockManager(this);
    this->end();
}