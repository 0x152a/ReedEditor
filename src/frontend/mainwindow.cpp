#include "mainwindow.h"

MainWindow::MainWindow(Table& windowConfig)
    : Fl_Window(
          windowConfig["width"].value_or(400),
          windowConfig["height"].value_or(400),
          windowConfig["title"].value_or("Reed Editor")),
      config(windowConfig)
{
}