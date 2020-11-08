#ifndef __DE_PLAYER_H
#define __DE_PLAYER_H

#include <windows.h>
#include <GL\gl.h>
#include <math.h>
#include "de_game_object.h"
#include "de_projectile_manager.h"
#include "de_keys.h"
#include "de_weapon.h"

//player class
class dePlayer: public deGameObject
{
private:
  deProjectileManager *projMan; //projectile manager
  deWeapon *weapon;             //weapon
  bool allowedShooting;         //player is allowed shooting flag
  bool allowedElementChange;    //player is allowed changing element flag
public:
  //init values
  dePlayer()
  {
    speed = 50;
    allowedShooting = true;
    allowedElementChange = true;
  }
  //draw player on the screen
  void draw();
  //set projectile manager
  void setProjectileManager(deProjectileManager *new_projMan) {projMan = new_projMan;}
  //move player based on direction and time
  void move(const int direction, const double time);
  //shoot in GO's direction
  void shootAt(const deGameObject *obj);
  //set current weapon
  void setWeapon(deWeapon *new_weapon) {weapon = new_weapon;}
  //allow/deny shooting
  void allowShooting() {allowedShooting = true;}
  void denyShooting() {allowedShooting = false;}
  //allow/deny changing element
  void allowElementChanging() {allowedElementChange = true;}
  void denyElementChanging() {allowedElementChange = false;}
  //is shooting allowed?
  bool isAllowedShooting() {return allowedShooting;}
  //is element changing allowed?
  bool isAllowedElementChanging() {return allowedElementChange;}
  //decrease hp on hit
  void hit(const int damage)
  {
    hp -= damage;
  }
  //read access
  deWeapon *getCurrentWeapon() {return weapon;}
};

#endif