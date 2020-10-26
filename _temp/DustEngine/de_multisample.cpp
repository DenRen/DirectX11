#include "de_multisample.h"

#include <windows.h>
#include <GL\gl.h>
#include <GL\glu.h>

#define WGL_SAMPLE_BUFFERS_ARB  0x2041
#define WGL_SAMPLES_ARB         0x2042

bool deMultisample::initMultisample(HINSTANCE hInstance, HWND hWnd, PIXELFORMATDESCRIPTOR pfd)
{  
  //check if multisample exists
  if (!GLEE_WGL_EXT_multisample)
  {
    arbMultisampleSupported = false;
    return false;
  }
  
  //get pixel format
  if (!GLEE_WGL_ARB_pixel_format)
  {
    arbMultisampleSupported = false;
    return false;
  }

  //get DC
  HDC hDC = GetDC(hWnd);

  int pixelFormat;
  int valid;
  UINT numFormats;
  float fAttributes[] = {0,0};

  //filling attributes
  int iAttributes[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
    WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 24,
    WGL_ALPHA_BITS_ARB, 8,
    WGL_DEPTH_BITS_ARB, 16,
    WGL_STENCIL_BITS_ARB, 0,
    WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
    WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
    WGL_SAMPLES_ARB, 4,
    0, 0
  };

  //trying to get 4 samples pixel format
  valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
  
  //if true and format count >= 1
  if (valid && numFormats >= 1)
  {
    arbMultisampleSupported = true;
    arbMultisampleFormat = pixelFormat;	
    return arbMultisampleSupported;
  }

  //trying to get 2 samples pixel format
  iAttributes[19] = 2;
  valid = wglChoosePixelFormatARB(hDC, iAttributes, fAttributes, 1, &pixelFormat, &numFormats);
  if (valid && numFormats >= 1)
  {
    arbMultisampleSupported = true;
    arbMultisampleFormat = pixelFormat;	 
    return arbMultisampleSupported;
  }
  //return valid format
  return  arbMultisampleSupported;
}