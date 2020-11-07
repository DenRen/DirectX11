#ifndef __DE_PHYSICS_MANAGER_H
#define __DE_PHYSICS_MANAGER_H

#include <list>
#include <math.h>
#include "de_projectile.h"
#include "de_player.h"
#include "de_enemy_manager.h"
#include "de_collision.h"

//physics maanger - hadles moving and collision of GOs
class dePhysicsManager
{
private:
  //pointer at player
  dePlayer *player;
  //pointers at projectiles and enemies lists
  std::list<deProjectile> *lstProj;
  std::list<deEnemy> *lstEnemies;
  //own collisions list
  std::list<deCollision> lstCollisions;
  //check if collision exists in list
  bool checkIfCollisionExists(const deGameObject *obj, const deProjectile *proj);
  //check if given GO doesn't collide with enemies and player (except itself)
  bool checkPlacementCollision(deGameObject *obj);
  //correct player placement
  void correctPlayerPlacement(const int direction, const double time);
  //move GOs
  void movePlayer(const double time, const int direction);
  void moveEnemies(const double time);
  void moveProj(const double time);
  //check if there are collisions, add new, check current
  void checkCollisions();
  //process new collisions and set them old
  void procNewCollisions();
  //cleanup old collisions
  void cleanupOldCollisions();
public:
  //set all the three pointers
  void init(dePlayer *new_player, std::list<deProjectile> *new_lstProj,
    std::list<deEnemy> *new_lstEnemies)
  {
    player = new_player;
    lstProj = new_lstProj;
    lstEnemies = new_lstEnemies;
  }
  //process collisions
  void processCollisions()
  {
    checkCollisions();
    procNewCollisions();
    cleanupOldCollisions();
  }
  //move all GOs
  void moveAll(const double time, const int playerDirection);
  //return collisions count
  int collisionsCount() {return lstCollisions.size();}
};

#endif