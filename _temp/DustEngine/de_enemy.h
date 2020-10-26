#ifndef __DE_ENEMY_H
#define __DE_ENEMY_H

#include <math.h>
#include <windows.h>
#include <GL\gl.h>
#include "de_routines.h"
#include "de_game_object.h"

//enemy moving types
const int DE_ENEMY_MOVING_TYPE_STATIC = 0;
const int DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER = 1;

//enemy visual types
const int DE_ENEMY_VISUAL_TYPE_SQUARE = 0;

//basic enemy class
class deEnemy: public deGameObject
{
private:
  int movingType;      //how it moves
  int visualType;      //how it looks
  double r, g, b;      //and what color is it
  bool dead;           //dead enemy flag
public:
  //create enemy with given attributes
  deEnemy(const int new_movingType, const int new_visualType,
    double new_r, double new_g, double new_b, const int new_elementType);
  deEnemy(deEnemy *new_enemy)
  {
    movingType = new_enemy->movingType;
    visualType = new_enemy->visualType;
    r = new_enemy->r;
    g = new_enemy->g;
    b = new_enemy->b;
    elementType = new_enemy->elementType;
    dead = new_enemy->dead;
    collRadius = new_enemy->collRadius;
    angle = new_enemy->angle;
    x = new_enemy->x;
    y = new_enemy->y;
    hpMax = new_enemy->hpMax;
  }
  //draw enemy on the screen
  void draw();
  //hit enemy
  void hit(const int damage)
  {
    //decrease hp
    hp -= damage;
    if (hp <= 0)
    {
      //flag it, if it's dead
      hp = 0;
      dead = true;
    }
  }
  void heal(const int value)
  {
    //increase hp
    hp += value;
    if (hp > hpMax)
      hp = hpMax;
  }
  //change color to a duller one
  void makeColorDull()
  {
    r = (1 - r) * 0.6 + r;
    g = (1 - g) * 0.6 + g;
    b = (1 - b) * 0.6 + b;
  }
  //draw a hit points bar in the upper part of the enemy on screen
  void drawHPBar();
  //set enemy's coordinates based on moving type and given arguments
  void move(const double time, const double arg1, const double arg2);
  //correct enemy movement
  void correct(const double time, const double arg1, const double arg2, const double correctionAngle);
  //read access
  bool isDead() {return dead;}
  int getMovingType() {return movingType;}
};

#endif