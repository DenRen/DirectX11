#ifndef __DE_WEAPON_H
#define __DE_WEAPON_H

#include "de_elements.h"
#include "de_projectile.h"
#include "de_projectile_manager.h"

//damage types
const int DE_DAMAGE_TYPE_CONSTANT = 0; //constant damage

//penetration level constant
const int DE_INFINITE_PENETRATIONS = -1; //infinite number of penetrations per projectile

//weapon class
class deWeapon
{
private:
  int damageType;               //damage type
  int elementType;              //element type
  int nextElementType;          //next element type
  int damageMin;                //minimum damage
  int damageMax;                //maximum damage
  double collisionRadius;       //collision radius
  double speed;                 //projectile speed
  double accel;                 //projectile acceleration
  double maxDeflection;         //maximum angle deflection
  int penetrateLevel;           //level of penetration (number of possible penetrations per projectile)
  int numberOfProjectiles;      //number of projectiles fired in a single shot
  double waitingTime;           //waiting time between two shots
  double r, g, b;               //projectile color
  int projMovingType;           //projectile moving type
  //TODO: make const
  int weaponId;                 //weapon id
  int targetType;               //target type
  deProjectileManager *projMan; //pointer at projectile maanger
public:
  //create new weapon
  deWeapon(const int new_weaponId, const int new_damageType, const int new_elementType, const int new_damageMin,
    const int new_damageMax, const double new_collisionRadius, const double new_speed, const double new_accel,
    const double new_maxDeflection, const int new_penetrateLevel, const int new_numberOfProjectiles,
    const double new_waitingTime,
    const double new_r, const double new_g, const double new_b, const int new_projMovingType, const int new_targetType)
  {
    weaponId = new_weaponId;

    damageType = new_damageType;
    elementType = new_elementType;
    do {
      nextElementType = getElementTypeByIndex(rand() % DE_ELEMENTS_COUNT);
    } while (nextElementType == elementType);
    damageMin = new_damageMin;
    damageMax = new_damageMax;
    collisionRadius = new_collisionRadius;
    speed = new_speed;
    accel = new_accel;
    maxDeflection = new_maxDeflection;
    penetrateLevel = new_penetrateLevel;
    numberOfProjectiles = new_numberOfProjectiles;
    waitingTime = new_waitingTime;
    r = new_r;
    g = new_g;
    b = new_b;
    projMovingType = new_projMovingType;
    targetType = new_targetType;
  }
  //set projectile manager
  void setProjectileManager(deProjectileManager *new_projMan) {projMan = new_projMan;}
  //shoot at direction of point described by it's coordinates
  void shoot(const double angle, const double x, const double y);
  //change elements
  void changeElements()
  {
    elementType = nextElementType;
    getElementColor(elementType, &r, &g, &b);
    do {
      nextElementType = getElementTypeByIndex(rand() % DE_ELEMENTS_COUNT);
    } while (nextElementType == elementType);
  }
  //read access
  int getElementType() {return elementType;}
  int getNextElementType() {return nextElementType;}
  double getWaitingTime() {return waitingTime;}
};

#endif