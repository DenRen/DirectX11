#ifndef __DE_COLLISION_H
#define __DE_COLLISION_H

#include "de_game_object.h"
#include "de_projectile.h"

//basic collision class
class deCollision
{
private:
  deGameObject *obj;    //pointer at GO that is hit by the projectile
  deProjectile *proj;   //that projectile
  bool _new;            //new collision flag
  bool checkedThisTime; //if existing collision isn't checked in another cycle, it'll be deleted
public:
  explicit deCollision(deGameObject *new_obj, deProjectile *new_proj)
  {
    obj = new_obj;
    proj = new_proj;
    _new = true;
    checkedThisTime = false;
  }
  //read access
  deGameObject *getObj() {return obj;}
  deProjectile *getProj() {return proj;}
  bool isNew() {return _new;}
  void makeOld() {_new = false;}
  bool isCheckedThisTime() {return checkedThisTime;}
  //write access
  void check() {checkedThisTime = true;}
  void uncheck() {checkedThisTime = false;}
};

#endif