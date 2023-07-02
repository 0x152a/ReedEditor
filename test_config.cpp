#include "config.h"
#include <iostream>

int main() {
  Config c;
  std::cout << c.require<int>("editor.ui", "font_size") << std::endl;
  return 0;
}