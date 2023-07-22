#pragma once

#include "mainwindow.h"
#include "reed.h"

class Frontend 
{
public:
    Frontend(Reed *reed);
    void show();
private:
    Reed *reed;
    MainWindow *mainWindow;
};