#include "TextEditor.h"

int ma2in(int argc, char** argv)
{
    Config* config = getConfig();
    // textbuf->transcoding_warning_action = NULL;

    Fl_Window* window = newView();
    // fl_open_callback(window->loadFileCB);

    window->show(1, argv);

    auto status = Fl::run();
    return status;
}
