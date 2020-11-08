#include "de_physics_manager.h"
#include "de_elements.h"
#include <math.h>

//check if collision exists in list
bool dePhysicsManager::checkIfCollisionExists(const deGameObject *obj, const deProjectile *proj)
{
  for (std::list<deCollision>::iterator idxColl = lstCollisions.begin(); idxColl != lstCollisions.end(); ++idxColl)
  {
    if (idxColl->getProj() == proj && idxColl->getObj() == obj)
    {
      //check existed
      idxColl->check();
      return true;
    }
  }
  return false;
}

//check if given GO doesn't collide with enemies and player (except itself)
bool dePhysicsManager::checkPlacementCollision(deGameObject *obj)
{
  //note: collision exists if sum of two GO collision radii > then distance between GO centers
  //check collision with enemies
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies->begin(); idxEnemy != lstEnemies->end(); ++idxEnemy)
  {
    if (!idxEnemy->isDead() && idxEnemy->self() != obj && (idxEnemy->distance(obj->self()) <
      (idxEnemy->getCollisionRadius() + obj->getCollisionRadius())))
      return true;
  }
  //check collision with player
  if (player != obj && (player->distance(obj->self()) <
    (player->getCollisionRadius() + obj->getCollisionRadius())))
    return true;
  return false;
}

//redirect player move
void dePhysicsManager::movePlayer(const double time, const int direction)
{
  player->move(direction, time);
}

//move enemies, considering collision between them and player
void dePhysicsManager::moveEnemies(const double time)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies->begin(); idxEnemy != lstEnemies->end(); ++idxEnemy)
  {
    //if is alive
    if (!idxEnemy->isDead())
    {
      //move it based on it's moving type
      switch (idxEnemy->getMovingType())
      {
      case DE_ENEMY_MOVING_TYPE_STATIC:
        idxEnemy->move(time, 0, 0);
        break;
      case DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER:
        //if is close to player - do not move
        if (idxEnemy->distance(player->self()) <
          (idxEnemy->getCollisionRadius() + player->getCollisionRadius() + DE_MELEE_ATTACK_RANGE))
        {
          //testing stuff
          player->hit(1);
          //
          continue;
        }
        //here we will need correction if there is collision with default movement
        idxEnemy->move(time, player->getX(), player->getY());
        //check if there is a collision
        if (checkPlacementCollision(idxEnemy->self()))
        {
          int bestAngle = 180;
          double bestDistance;
          idxEnemy->restorePosition();
          bestDistance = idxEnemy->distance(player);
          //start correcting clockwise by 10 degrees
          for (int angle = 0; angle < 360; angle += 10)
          {
            idxEnemy->correct(time, player->getX(), player->getY(), angle);
            //if place is free
            if (!checkPlacementCollision(idxEnemy->self()))
            {
              if (idxEnemy->distance(player) < bestDistance)
              {
                bestDistance = idxEnemy->distance(player);
                bestAngle = angle;
              }
            }
          }
          //if there is a place to step
          if (bestAngle != 180)
          {
            //apply correction
            idxEnemy->correct(time, player->getX(), player->getY(), bestAngle);
          }
          else
          {
            //if angle = 350, there is no place to go without collision, so stay
            idxEnemy->restorePosition();
          }
          //rotate enemy to face the player
          idxEnemy->lookAt(player);
        }
        break;
      }
    }
  }
}

//redirect projectiles move
void dePhysicsManager::moveProj(const double time)
{
  for (std::list<deProjectile>::iterator i = lstProj->begin(); i != lstProj->end(); ++i)
    i->move(time);
}

//move all GOs
void dePhysicsManager::moveAll(const double time, const int playerDirection)
{
  movePlayer(time, playerDirection);
  //if player is colliding with an enemy, correct his position
  if (checkPlacementCollision(player))
    correctPlayerPlacement(playerDirection, time);
  moveProj(time);
  moveEnemies(time);
}

//check if there are collisions, add new, check current
void dePhysicsManager::checkCollisions()
{
  //for all the projectiles
  for (std::list<deProjectile>::iterator idxProj = lstProj->begin(); idxProj != lstProj->end(); ++idxProj)
  {
    //depending on it's target type
    switch (idxProj->getTargetType())
    {
    case DE_TARGET_TYPE_PLAYER:
      //check collision with player
      if (player->distance(idxProj->self()) <
        (player->getCollisionRadius() + idxProj->getCollisionRadius()))
      {
        //if no such collision exists in list - add it
        if (!checkIfCollisionExists(player, dynamic_cast<deProjectile*>(idxProj->self())))
        {  
          deCollision collision(player, dynamic_cast<deProjectile*>(idxProj->self()));
          lstCollisions.push_back(collision);
          collision.~deCollision();
        }
      }
      break;
    case DE_TARGET_TYPE_ENEMY:
      //same with all the enemies
      for (std::list<deEnemy>::iterator idxEnemy = lstEnemies->begin(); idxEnemy != lstEnemies->end(); ++idxEnemy)
      {
        if (!idxEnemy->isDead() && (idxEnemy->distance(idxProj->self()) <
          (idxEnemy->getCollisionRadius() + idxProj->getCollisionRadius())))
        {
          if (checkIfCollisionExists(idxEnemy->self(), dynamic_cast<deProjectile*>(idxProj->self())))
          {
            continue;
          }
          deCollision collision(idxEnemy->self(), dynamic_cast<deProjectile*>(idxProj->self()));
          lstCollisions.push_back(collision);
          collision.~deCollision();
        }
      }
      break;
    case DE_TARGET_TYPE_ALL:
      //will be added later
      break;
    }
  }
}

//process new collisions and set them old
void dePhysicsManager::procNewCollisions()
{
  for (std::list<deCollision>::iterator idxColl = lstCollisions.begin(); idxColl != lstCollisions.end(); ++idxColl)
  {
    //if is new
    if (idxColl->isNew())
    {
      //execute stuff here

      //reduce number of allowed penetrations
      if (idxColl->getProj()->getPenetrations() != DE_INFINITE_PENETRATIONS)
        idxColl->getProj()->penetrate();
      //
      double modifier = 0.0;
      int damage = 0;
      modifier = elementalDamageModifier(idxColl->getProj()->getElementType(), idxColl->getObj()->getElementType());
      damage = (int)floor(idxColl->getProj()->getDamage() * modifier);
      if (modifier == DE_DAMAGE_MODIFIER_HEAL)
        idxColl->getObj()->heal(damage);
      else
        idxColl->getObj()->hit(damage);

      //TEST STUFF

      //make some projectiles at boss' death
      if (dynamic_cast<deEnemy*>(idxColl->getObj())->isDead() && idxColl->getObj()->getHPMax() == 500)
      {
        double r, g, b;
        int elementType = idxColl->getObj()->getElementType();
        getElementColor(elementType, &r, &g, &b);
        double x, y;
        x = idxColl->getObj()->getX();
        y = idxColl->getObj()->getY();
        double angle = 0.0;
        for (int i = 0; i < 36; ++i)
        {
          int id;
          lstProj->push_back(deProjectile(DE_PROJECTILE_MOVING_TYPE_STRAIGHT, DE_PROJECTILE_VISUAL_TYPE_SQUARE,
            r, g, b, elementType));
          lstProj->back().setXY(x, y);
          lstProj->back().setAngle(angle);
          lstProj->back().setCollisionRadius(4.0);
          lstProj->back().setTargetType(DE_TARGET_TYPE_ENEMY);
          lstProj->back().setSpeed(500);
          lstProj->back().setAccel(500);
          lstProj->back().setDamage(25);
          lstProj->back().setPenetrations(1);
          angle += 10;
        }
      }

      //
      
      //make old and check
      idxColl->makeOld();
      idxColl->check();
    }
  }
}

//cleanup old collisions
void dePhysicsManager::cleanupOldCollisions()
{
  //delete unchecked
  std::list<deCollision>::iterator idxColl = lstCollisions.begin();
  while (idxColl != lstCollisions.end())
  {
    if (!idxColl->isCheckedThisTime())
    {
      lstCollisions.erase(idxColl);
      idxColl = lstCollisions.begin();
    }
    else
      ++idxColl;
  }

  //uncheck remaining
  for (idxColl = lstCollisions.begin(); idxColl != lstCollisions.end(); ++idxColl)
    idxColl->uncheck();
}

//correct player placement
void dePhysicsManager::correctPlayerPlacement(const int direction, const double time)
{
  //"unmove" player
  player->restorePosition();
  //try to place player in one of the two adjoining directions, with priority to CCW
  switch (direction)
  {
  case DE_DIRECTION_N:
    player->move(DE_DIRECTION_NW, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_NE, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_NE:
    player->move(DE_DIRECTION_N, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_E, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_E:
    player->move(DE_DIRECTION_NE, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_SE, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_SE:
    player->move(DE_DIRECTION_E, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_S, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_S:
    player->move(DE_DIRECTION_SE, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_SW, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_SW:
    player->move(DE_DIRECTION_S, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_W, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_W:
    player->move(DE_DIRECTION_SW, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_NW, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_NW:
    player->move(DE_DIRECTION_W, time);
    if (checkPlacementCollision(player))
    {
      player->restorePosition();
      player->move(DE_DIRECTION_N, time);
      if (checkPlacementCollision(player))
      {
        player->restorePosition();
      }
    }
    break;
  case DE_DIRECTION_IDLE:
    break;
  }
}