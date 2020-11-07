#include "de_enemy.h"

//create enemy with given attributes
deEnemy::deEnemy(const int new_movingType, const int new_visualType,
    double new_r, double new_g, double new_b, const int new_elementType)
{
  movingType = new_movingType;
  visualType = new_visualType;
  r = new_r;
  g = new_g;
  b = new_b;
  elementType = new_elementType;
  //it isn't dead and wasn't corrected
  dead = false;
}

//draw enemy on the screen
void deEnemy::draw()
{
  //draw it based on visual type
  switch (visualType)
  {
  case DE_ENEMY_VISUAL_TYPE_SQUARE:
    //simple square
    //backup view
    glPushMatrix();
    //move view to enemy's center
    glTranslatef((GLfloat)x, (GLfloat)y, 0.0);
    //rotate view
    glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
    //set brush color
    glColor3f((GLfloat)r, (GLfloat)g, (GLfloat)b);
    //draw square
    glRectf((GLfloat)collRadius, (GLfloat)collRadius, (GLfloat)-collRadius, (GLfloat)-collRadius);
    //restore view
    glPopMatrix();
    break;
  };
}

//draw a hit points bar in the upper part of the enemy on screen
void deEnemy::drawHPBar()
{
  //calculate hp cur/max percentage
  double percentage = (double)hp / hpMax;
  //backup view
  glPushMatrix();
  //move view to middle upper part of enemy
  glTranslatef((GLfloat)x, (GLfloat)(y - collRadius - 4), 0.0);
  //set hp bar color based on hp percentage (0 = red, 1 = green, (r + g) = 1, b = 0)
  glColor3f((GLfloat)(1 - percentage), (GLfloat)percentage, 0.0);
  //draw hp bar from left upper part of enemy to the right (bar length also based on hp percentage)
  glRectf((GLfloat)(-collRadius + 2 * collRadius * percentage), (GLfloat)2,
    (GLfloat)(-collRadius), (GLfloat)-2);
  //restore view
  glPopMatrix();
}

//set enemy's coordinates based on moving type and given arguments
void deEnemy::move(const double time, const double arg1, const double arg2)
{
  //backup current position
  backupPosition();
  //move enemy based on moving type
  switch (movingType)
  {
  case DE_ENEMY_MOVING_TYPE_STATIC:
    //do not move
    break;
  case DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER:
    //move to player
    //arguments - x, y of player

    //set angle towards player
    lookAt(arg1, arg2);
    //calculate moving distance
    double distance = speed * time;
    //apply it
    x += distance * cos(deg2rad(angle));
    y += distance * sin(deg2rad(angle));
    break;
  }
}

//correct enemy movement
void deEnemy::correct(const double time, const double arg1, const double arg2, const double correctionAngle)
{
  //based on enemy's moving type
  switch (movingType)
  {
  case DE_ENEMY_MOVING_TYPE_STATIC:
    //corrections aren't needed
    break;
  case DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER:
    //if moving towards player
    //arguments - x, y of player

    //set angle towards player
    lookAt(arg1, arg2);
    //and correct it
    angle += correctionAngle;
    //calculate distance and apply it
    double distance = speed * time;
    x = oldX + distance * cos(deg2rad(angle));
    y = oldY + distance * sin(deg2rad(angle));
    break;
  }
}