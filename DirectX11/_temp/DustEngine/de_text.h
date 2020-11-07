#ifndef __DE_TEXT_H
#define __DE_TEXT_H

#include <windows.h>
#include <GL\gl.h>
#include <string.h>

//text display class
class deText
{
private:
  GLuint nFontList; //font starting index in display lists
public:
  //create font outlines
  void initFont();
  //display text
  void printText(const double x, const double y, const char *text);
};

#endif