#ifndef __DE_EVENT_MANAGER_H
#define __DE_EVENT_MANAGER_H

#include <list>
#include "de_event.h"
#include "de_timer.h"
#include "de_enemy_manager.h"
#include "de_player.h"

//event manager class
class deEventManager
{
  static int idGenerator;      //generator of id's
  std::list<deEvent> lstEvents; //list of events
  deEnemyManager *enemyMan;     //pointer at enemy manager
  dePlayer *player;             //pointer at player
public:
  //reset id generator
  deEventManager() {idGenerator = 0;}
  //add new event
  int addEvent(const int new_type, const int new_action,
    const double new_duration, const int new_data = 0);
  //update all events
  void updateEvents(const double time);
  //stop all events and dlete them from list
  void stopAll()
  {
    lstEvents.clear();
  }
  //events count
  int eventsCount()
  {
    return lstEvents.size();
  }
  //execute event's action
  void executeAction(deEvent event)
  {
    //execute action based on its type
    switch (event.getAction())
    {
    case DE_EVENT_ACTION_SPAWN_ENEMY:
      onSpawnEnemy();
      break;
    case DE_EVENT_ACTION_SPAWN_MINIBOSS:
      onSpawnMiniboss();
      break;
    case DE_EVENT_ACTION_SPAWN_BOSS:
      onSpawnBoss();
      break;
    case DE_EVENT_ACTION_DENY_SHOOTING:
      onDenyShooting();
      break;
    case DE_EVENT_ACTION_DENY_ELEMENT_CHANGING:
      onDenyElementChanging();
      break;
    }
  }
  //set pointers on other engine parts
  void setEnemyManager(deEnemyManager *new_enemyMan) {enemyMan = new_enemyMan;}
  void setPlayer(dePlayer *new_player) {player = new_player;}
  //action funcs
  void onSpawnEnemy();
  void onSpawnMiniboss();
  void onSpawnBoss();
  void onDenyShooting();
  void onDenyElementChanging();
};

#endif