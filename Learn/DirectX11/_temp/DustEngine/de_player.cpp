#include "de_player.h"

#include "de_enemy_manager.h"

//draw player on the screen
void dePlayer::draw()
{
  //backup view
  glPushMatrix();
  //move view to player's position
  glTranslatef((GLfloat)x, (GLfloat)y, 0.0);
  //rotate view on player's angle
  glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
  //set player color black
  glColor3f(0.0, 0.0, 0.0);
  //draw rectangle
  glRectf((GLfloat)collRadius, (GLfloat)collRadius, (GLfloat)-collRadius, (GLfloat)-collRadius);
  //restore view
  glPopMatrix();
}

//move player based on direction and time
void dePlayer::move(const int direction, const double time)
{
  backupPosition();

  //calculate distance and it's proection for NW\NE\SW\SE directions
  double distance = speed * time;
  double proection = distance / sqrt((double)2);

  //move player based on given discrete direction
  switch (direction)
  {
  case DE_DIRECTION_N:
    y -= distance;
    break;
  case DE_DIRECTION_NE:
    y -= proection;
    x += proection;
    break;
  case DE_DIRECTION_E:
    x += distance;
    break;
  case DE_DIRECTION_SE:
    y += proection;
    x += proection;
    break;
  case DE_DIRECTION_S:
    y += distance;
    break;
  case DE_DIRECTION_SW:
    y += proection;
    x -= proection;
    break;
  case DE_DIRECTION_W:
    x -= distance;
    break;
  case DE_DIRECTION_NW:
    y -= proection;
    x -= proection;
    break;
  case DE_DIRECTION_IDLE:
    break;
  }
  //deny crossing the game screen border
  if (x < 0)
    x = 0;
  if (x > 640)
    x = 640;
  if (y < 0)
    y = 0;
  if (y > 480)
    y = 480;
  return;
}

//shoot in GO's direction
void dePlayer::shootAt(const deGameObject *obj)
{
  //shoot if allowed
  if (allowedShooting)
    weapon->shoot(getAngleAt(obj), x, y);
}