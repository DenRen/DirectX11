#include "de_projectile.h"

//create projectile with given params
deProjectile::deProjectile(const int new_movingType, const int new_visualType,
    const double new_r, const double new_g, const double new_b, const int new_elementType)
{
  movingType = new_movingType;
  visualType = new_visualType;
  r = new_r;
  g = new_g;
  b = new_b;
  elementType = new_elementType;
  //default
  damage = 0;
  unneeded = false;
  targetType = DE_TARGET_TYPE_ENEMY;
}

//draw projectile on screen
void deProjectile::draw()
{
  switch (visualType)
  {
  case DE_PROJECTILE_VISUAL_TYPE_SQUARE:
    //backup view
    glPushMatrix();
    //move view to projectile's position
    glTranslatef((GLfloat)x, (GLfloat)y, 0.0);
    //rotate by it's angle
    glRotatef((GLfloat)angle, 0.0, 0.0, 1.0);
    //set current color projectile color
    glColor3f((GLfloat)r, (GLfloat)g, (GLfloat)b);
    //draw rectangle
    glRectf((GLfloat)collRadius, (GLfloat)collRadius, (GLfloat)-collRadius, (GLfloat)-collRadius);
    //restore view
    glPopMatrix();
    break;
  };
}

//move projectile
void deProjectile::move(const double time)
{
  switch (movingType)
  {
  case DE_PROJECTILE_MOVING_TYPE_STRAIGHT:
    double hypotenuse; //moving distance
    //calculate it
    hypotenuse = speed * time + (accel * pow(time, 2) / 2);
    //increase coordinates by its proections
    x += hypotenuse * cos(deg2rad(angle));
    y += hypotenuse * sin(deg2rad(angle));
    //increase speed
    speed += accel * time;
    break;
  }
}