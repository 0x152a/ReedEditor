#include "frontend/frontend.h"
#include "reed.h"

int main(int argc, char **argv)
{
    Reed reed;
    // textbuf->transcoding_warning_action = NULL;
    Frontend frontend(&reed);

    /*
    Fl_Window* window = newView();

    // fl_open_callback(window->loadFileCB);

    window->show(1, argv);
    */

    frontend.show();
    int status = Fl::run();
    return status;
}
