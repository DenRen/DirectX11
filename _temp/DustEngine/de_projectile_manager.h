#ifndef __DE_PROJECTILE_MANAGER_H
#define __DE_PROJECTILE_MANAGER_H

#include <list>
#include "de_projectile.h"

//projectile manager
class deProjectileManager
{
private:
  static int idGenerator;                 //generator of ids
  std::list<deProjectile> lstProjectiles; //list of projectiles
public:
  //reset id generator
  deProjectileManager() {idGenerator = 0;}
  //add new projectile
  int add(const int new_movingType, const int new_visualType,
  const double new_r, const double new_g, const double new_b, const int new_elementType);
  //draw all the projectiles
  void draw()
  {
    for (std::list<deProjectile>::iterator idxProj = lstProjectiles.begin(); idxProj != lstProjectiles.end(); ++idxProj)
      idxProj->draw();
  }
  //move all the projectiles
  void move(const double time)
  {
    for (std::list<deProjectile>::iterator idxProj = lstProjectiles.begin(); idxProj != lstProjectiles.end(); ++idxProj)
      idxProj->move(time);
  }
  //write access
  void setPosition(const int id, const double new_x, const double new_y);
  void setAngle(const int id, const double new_angle);
  void setCollisionRadius(const int id, const double new_collRadius);
  void setSpeed(const int id, const double new_speed);
  void setAccel(const int id, const double new_accel);
  void setTargetType(const int id, const int new_targetType);
  void setDamage(const int id, const int new_damage);
  void setPenetrations(const int id, const int new_penetrations);
  //delete projectiles beyond the game field and with penetrations = 0
  void cleanup(const int left, const int top, const int width, const int height);
  //return projeciles count
  int count() {return lstProjectiles.size();}
  //return pointer at projectiles list
  std::list<deProjectile> *getListPointer() {return &lstProjectiles;}
  void clear() {lstProjectiles.clear();}
};

#endif