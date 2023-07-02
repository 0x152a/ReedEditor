#include "TextEditor.h"

int main(int argc, char **argv) {
  // textbuf->transcoding_warning_action = NULL;
  fl_open_callback(loadFileCB);

  Fl_Window *window = newView();

  window->show(1, argv);

  return Fl::run();
}
