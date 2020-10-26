#ifndef __DE_MULTISAMPLE_H
#define __DE_MULTISAMPLE_H

#include <glee.h>

class deMultisample
{
private:
  bool arbMultisampleSupported;
  int arbMultisampleFormat;
public:
  bool isSupported() {return arbMultisampleSupported;};
  int getFormat() {return arbMultisampleFormat;};
  bool initMultisample(HINSTANCE hInstance, HWND hWnd, PIXELFORMATDESCRIPTOR pfd);
};

#endif