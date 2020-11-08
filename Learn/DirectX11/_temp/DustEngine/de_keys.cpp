#include "de_keys.h"

//get player direction based on moving controls
int deKeys::getPlayerDirection()
{
  /*
  NW  N  NE
    \ | / 
  W - 0 - E
    / | \ 
  SW  S  SE
  */
  //system is very simple:
  //if pressed left or right then set horz -1 or +1
  //if neither left nor right are pressed then set horz 0
  //if both are pressed - also 0
  //same with up, down and vert
  //up -1, down +1, 0 in other cases
  int vert, horz;
  vert = horz = 0;
  //calculate vert and horz
  if (keys[DE_KEY_GO_UP])
    --vert;
  if (keys[DE_KEY_GO_DOWN])
    ++vert;
  if (keys[DE_KEY_GO_LEFT])
    --horz;
  if (keys[DE_KEY_GO_RIGHT])
    ++horz;
  //calculate direction based on vert and horz
  if (!vert && !horz)
    return DE_DIRECTION_IDLE;
  else if (!vert && horz > 0)
    return DE_DIRECTION_E;
  else if (!vert && horz < 0)
    return DE_DIRECTION_W;
  else if (vert > 0 && !horz)
    return DE_DIRECTION_S;
  else if (vert > 0 && horz > 0)
    return DE_DIRECTION_SE;
  else if (vert > 0 && horz < 0)
    return DE_DIRECTION_SW;
  else if (vert < 0 && !horz)
    return DE_DIRECTION_N;
  else if (vert < 0 && horz > 0)
    return DE_DIRECTION_NE;
  else
    return DE_DIRECTION_NW;
}