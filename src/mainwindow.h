#pragma once

#include "codeeditor.h"
#include "reed.h"

class MainWindow
{
public:
    MainWindow();
private:
    CodeEditor *editor;
    Reed *app;
};