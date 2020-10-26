#ifndef __DE_CURSOR_H
#define __DE_CURSOR_H

#include <windows.h>
#include <GL\gl.h>
#include "de_game_object.h"

//software game cursor class
class deCursor: public deGameObject
{
public:
  //initialization
  deCursor() {angle = 45; collRadius = 1;};
  //draw cursor on the screen
  void draw();
  //set cursor's coordinates
  void move(const int new_x, const int new_y)
  {
    x = new_x;
    y = new_y;
  }
};

#endif