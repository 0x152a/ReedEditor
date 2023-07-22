#include "frontend.h"

Frontend::Frontend(Reed *reed)
{
    this->reed = reed;
    this->mainWindow = new MainWindow(reed->getMainWindowConfig());
}

void Frontend::show()
{
    mainWindow->show();
}

