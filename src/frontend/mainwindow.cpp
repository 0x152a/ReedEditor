#include "mainwindow.h"

MainWindow::MainWindow(Table& config)
    : Fl_Window(
          config["main-window"]["width"].value_or(400),
          config["main-window"]["height"].value_or(400),
          config["main-window"]["title"].value_or("Reed Editor")),
      config(config)
{
}