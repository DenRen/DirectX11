#include "de_enemy_manager.h"

int deEnemyManager::idGenerator; //generator of ids

//add new enemy to manager
int deEnemyManager::add(const int new_movingType, const int new_visualType, double new_r, double new_g, double new_b,
                        const int new_elementType)
{
  //create enemy and set an id
  deEnemy enemy(new_movingType, new_visualType, new_r, new_g, new_b, new_elementType);
  enemy.setId(idGenerator++);
  //add it to the list
  lstEnemies.push_back(enemy);
  //return it's id
  return enemy.getId();
}

//set various enemies params
//principle of every func of that group - find an enemy with given id and modify the param
void deEnemyManager::setPosition(const int id, const double new_x, const double new_y)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    if (idxEnemy->getId() == id)
    {
      idxEnemy->setXY(new_x, new_y);
      return;
    }
}

void deEnemyManager::rotate(const int id, const double new_angle)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    if (idxEnemy->getId() == id)
    {
      idxEnemy->setAngle(new_angle);
      return;
    }
}

void deEnemyManager::setCollisionRadius(const int id, const double new_collRadius)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    if (idxEnemy->getId() == id)
    {
      idxEnemy->setCollisionRadius(new_collRadius);
      return;
    }
}

void deEnemyManager::setSpeed(const int id, const double new_speed)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    if (idxEnemy->getId() == id)
    {
      idxEnemy->setSpeed(new_speed);
      return;
    }
}

void deEnemyManager::setHP(const int id, const int new_hpMax)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    if (idxEnemy->getId() == id)
    {
      idxEnemy->setHPMax(new_hpMax);
      return;
    }
}

//hit enemy
void deEnemyManager::hit(const int id, const int damage)
{
  for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    if (idxEnemy->getId() == id)
    {
      //redirect hit() to the enemy
      idxEnemy->hit(damage);
      if (idxEnemy->isDead())
      {

      }
      return;
    }
}