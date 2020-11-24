#ifndef __DE_PROJECTILE_H
#define __DE_PROJECTILE_H

#include <math.h>
#include <windows.h>
#include <GL\gl.h>
#include "de_elements.h"
#include "de_routines.h"
#include "de_game_object.h"

//moving types
const int DE_PROJECTILE_MOVING_TYPE_STRAIGHT = 0;

//visual types
const int DE_PROJECTILE_VISUAL_TYPE_SQUARE = 0;

//target types
const int DE_TARGET_TYPE_PLAYER = 0;
const int DE_TARGET_TYPE_ENEMY = 1;
const int DE_TARGET_TYPE_ALL = 2;

//projectile class
class deProjectile: public deGameObject
{
private:
  int movingType;   //moving type
  int visualType;   //visual type
  double r, g, b;   //color
  int targetType;   //target type
  int damage;       //damage
  int penetrations; //penetrations remain
  bool unneeded; //projectile to be deleted flag
public:
  //create projectile with given params
  deProjectile(const int new_movingType, const int new_visualType,
    const double new_r, const double new_g, const double new_b, const int new_elementType);
  //draw projectile on screen
  void draw();
  //move projectile
  void move(const double time);
  //read access
  int getDamage() {return damage;}
  int getTargetType() {return targetType;}
  int getPenetrations() {return penetrations;}
  bool isToBeDeleted() {return unneeded;}
  //write access
  void setTargetType(const int new_targetType) {targetType = new_targetType;}
  void setDamage(const int new_damage) {damage = new_damage;}
  void setPenetrations(const int new_penetrations) {penetrations = new_penetrations;}
  void penetrate() {--penetrations;}
  void toBeDeleted() {unneeded = true;}
};

#endif