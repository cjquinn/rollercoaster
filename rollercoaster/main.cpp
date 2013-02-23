#include <windows.h>

#include "canvas.h"

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
  return Canvas::instance().processEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
  Canvas &canvas = Canvas::instance();
  canvas.setHInstance(hinstance);

  return canvas.exec();
}
