#ifndef __DE_KEYS_H
#define __DE_KEYS_H

#include "de_routines.h"

//keys count
const int DE_KEYS_COUNT = 256;

//moving controls - make this changeable
const char DE_KEY_GO_UP = 'W';
const char DE_KEY_GO_DOWN = 'S';
const char DE_KEY_GO_LEFT = 'A';
const char DE_KEY_GO_RIGHT = 'D';

//keys controls class
class deKeys
{
private:
  //pressed key flags array
  bool keys[DE_KEYS_COUNT];
  //pressed LMB flag
  bool leftMouseButton;
  //pressed RMB flag
  bool rightMouseButton;
public:
  //set all to false
  deKeys()
  {
    for (int i = 0; i < DE_KEYS_COUNT; ++i)
      keys[i] = false;
    leftMouseButton = false;
    rightMouseButton = false;
  }
  //press and unpress a key
  void keyDown(const char key) {keys[key] = true;}
  void keyUp(const char key) {keys[key] = false;}
  //press and unpress the LMB and RMB
  void downLMB() {leftMouseButton = true;}
  void upLMB() {leftMouseButton = false;}
  void downRMB() {rightMouseButton = true;}
  void upRMB() {rightMouseButton = false;}
  //is LMB and RMB pressed?
  bool isLMB() {return leftMouseButton;}
  bool isRMB() {return rightMouseButton;}
  //get player direction based on moving controls
  int getPlayerDirection();
  //read access
  bool isPressed(const char key) {return keys[key];}
};

#endif