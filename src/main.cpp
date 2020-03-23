#include "canvas.h"

int main(int argc, char *argv[]) {
  Canvas &canvas = Canvas::instance();
  return canvas.exec();
}
