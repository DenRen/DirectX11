#ifndef __DE_ENEMY_MANAGER_H
#define __DE_ENEMY_MANAGER_H

#include <list>
#include <vector>
#include "de_enemy.h"
#include "de_player.h"

//enemy manager
class deEnemyManager
{
private:
  static int idGenerator;          //generator of ids
  std::list<deEnemy> lstEnemies;   //list of enemies
  std::vector<deEnemy> vecCorpses; //vector of corpses
public:
  //reset id generator
  deEnemyManager() {idGenerator = 0;}
  //add new enemy to manager
  int add(const int new_movingType, const int new_visualType,
    double new_r, double new_g, double new_b, const int new_elementType);
  //draw all alive enemies
  void draw()
  {
    for (std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin(); idxEnemy != lstEnemies.end(); ++idxEnemy)
    {
      idxEnemy->draw();
      idxEnemy->drawHPBar();
    }
  }
  //draw all dead enemies
  void drawDead()
  {
    for (std::vector<deEnemy>::iterator idxEnemy = vecCorpses.begin(); idxEnemy != vecCorpses.end(); ++idxEnemy)
    {
      idxEnemy->draw();
    }
  }
  //set various enemies params
  void setPosition(const int id, const double new_x, const double new_y);
  void rotate(const int id, const double new_angle);
  void setCollisionRadius(const int id, const double new_collRadius);
  void setSpeed(const int id, const double new_speed);
  void setHP(const int id, const int new_hpMax);
  //hit enemy
  void hit(const int id, const int damage);
  //get pointer at enemies list
  std::list<deEnemy> *getListPointer() {return &lstEnemies;}
  //all enemies count
  int count() {return lstEnemies.size();}
  //dead enemies count
  int corpsesCount() {return vecCorpses.size();}
  //player score
  int score()
  {
    int result = 0;
    //add to result every dead enemy's hp
    for (std::vector<deEnemy>::iterator idxEnemy = vecCorpses.begin(); idxEnemy != vecCorpses.end(); ++idxEnemy)
    {
      result += idxEnemy->getHPMax();
    }
    return result;
  }
  void clear()
  {
    lstEnemies.clear();
    vecCorpses.clear();
  }
  //move dead enemies to corpses vector
  void buryTheDead()
  {
    std::list<deEnemy>::iterator idxEnemy = lstEnemies.begin();
    while (idxEnemy != lstEnemies.end())
    {
      if (idxEnemy->isDead())
      {
        deEnemy enemy(&(*idxEnemy));
        vecCorpses.push_back(enemy);
        vecCorpses.back().makeColorDull();
        lstEnemies.erase(idxEnemy);
        idxEnemy = lstEnemies.begin();
      }
      else
        ++idxEnemy;
    }
  }
};

#endif