#include "de_event_manager.h"
#include "de_elements.h"

int deEventManager::idGenerator; //generator of id's

//add new event
int deEventManager::addEvent(const int new_type, const int new_action,
    const double new_duration, const int new_data)
{
  //add event to list
  lstEvents.push_back(deEvent(new_type, new_action, new_duration, new_data));
  //add event id
  int id = idGenerator++;
  lstEvents.back().setId(id);
  //return it's id
  return id;
}

//update all events
void deEventManager::updateEvents(const double time)
{
  //check if some events are completed and increase their lasting time
  for (std::list<deEvent>::iterator idxEvnt = lstEvents.begin(); idxEvnt != lstEvents.end(); ++idxEvnt)
  {
    //if event's duration had passed
    if (idxEvnt->getLastingTime() >= idxEvnt->getDuration())
    {
      //execute action
      executeAction(*idxEvnt);
      switch (idxEvnt->getType())
      {
        //if event type is infinite start it again
        case DE_EVENT_TYPE_INFINITE:
          idxEvnt->startAgain();
          break;
        //if it's a canceller, complete it's target event
        case DE_EVENT_TYPE_EVENT_CANCELLER:
          for (std::list<deEvent>::iterator idxEvntSrch = lstEvents.begin(); idxEvntSrch != lstEvents.end(); ++idxEvntSrch)
          {
            if (idxEvntSrch->getId() == idxEvnt->getData())
            {
              idxEvntSrch->completeEvent();
              break;
            }
          }
        //complete event
        default:
          idxEvnt->completeEvent();
      }
    }
    //add lasting time
    idxEvnt->increaseTime(time);
  }

  //delete completed events
  std::list<deEvent>::iterator idxEvnt = lstEvents.begin();
  while (idxEvnt != lstEvents.end())
  {
    if (idxEvnt->isCompleted())
    {
      lstEvents.erase(idxEvnt);
      idxEvnt = lstEvents.begin();
    }
    else
      idxEvnt++;
  }
}

//action funcs:
//on enemy spawn
void deEventManager::onSpawnEnemy()
{
  //generate enemy's element
  int elementType = 0;
  switch (rand() % 4)
  {
  case 0:
    elementType = DE_ELEMENT_TYPE_FIRE;
    break;
  case 1:
    elementType = DE_ELEMENT_TYPE_WATER;
    break;
  case 2:
    elementType = DE_ELEMENT_TYPE_AIR;
    break;
  case 3:
    elementType = DE_ELEMENT_TYPE_EARTH;
    break;
  }
  int id;
  int hp = 50;
  double r, g, b;
  //set enemy color based on it's element
  getElementColor(elementType, &r, &g, &b);
  //add enemy to manager
  id = enemyMan->add(DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER, DE_ENEMY_VISUAL_TYPE_SQUARE, r, g, b, elementType);
  //set it's collision radius and hp
  enemyMan->setCollisionRadius(id, 15);
  enemyMan->setHP(id, hp);
  //set it position somewhere beyond the game screen, but now so far away
  switch (rand() % 4)
  {
  case 0:
    enemyMan->setPosition(id, rand() % 640, -40);
    break;
  case 1:
    enemyMan->setPosition(id, -40, rand() % 480);
    break;
  case 2:
    enemyMan->setPosition(id, 680, rand() % 480);
    break;
  case 3:
    enemyMan->setPosition(id, rand() % 640, 520);
    break;
  default:
    enemyMan->setPosition(id, rand() % 640, rand() % 480);
    break;
  }
  //set it's speed
  enemyMan->setSpeed(id, 40);
}

//on miniboss spawn
void deEventManager::onSpawnMiniboss()
{
  //generate enemy's element
  int elementType = 0;
  switch (rand() % 4)
  {
  case 0:
    elementType = DE_ELEMENT_TYPE_FIRE;
    break;
  case 1:
    elementType = DE_ELEMENT_TYPE_WATER;
    break;
  case 2:
    elementType = DE_ELEMENT_TYPE_AIR;
    break;
  case 3:
    elementType = DE_ELEMENT_TYPE_EARTH;
    break;
  }
  int id;
  int hp = 150;
  double collRadius = 30;
  double r, g, b;
  //set enemy color based on it's element
  getElementColor(elementType, &r, &g, &b);
  //add enemy to manager
  id = enemyMan->add(DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER, DE_ENEMY_VISUAL_TYPE_SQUARE, r, g, b, elementType);
  //set it's collision radius and hp
  enemyMan->setCollisionRadius(id, collRadius);
  enemyMan->setHP(id, hp);
  //set it position somewhere beyond the game screen, but now so far away
  switch (rand() % 4)
  {
  case 0:
    enemyMan->setPosition(id, rand() % 640, -40);
    break;
  case 1:
    enemyMan->setPosition(id, -40, rand() % 480);
    break;
  case 2:
    enemyMan->setPosition(id, 680, rand() % 480);
    break;
  case 3:
    enemyMan->setPosition(id, rand() % 640, 520);
    break;
  default:
    enemyMan->setPosition(id, rand() % 640, rand() % 480);
    break;
  }
  //set it's speed
  enemyMan->setSpeed(id, 30);
}

//on boss spawn
void deEventManager::onSpawnBoss()
{
  //generate enemy's element
  int elementType = 0;
  switch (rand() % 4)
  {
  case 0:
    elementType = DE_ELEMENT_TYPE_FIRE;
    break;
  case 1:
    elementType = DE_ELEMENT_TYPE_WATER;
    break;
  case 2:
    elementType = DE_ELEMENT_TYPE_AIR;
    break;
  case 3:
    elementType = DE_ELEMENT_TYPE_EARTH;
    break;
  }
  int id;
  int hp = 500;
  double collRadius = 50.0;
  double r, g, b;
  //set enemy color based on it's element
  getElementColor(elementType, &r, &g, &b);
  //add enemy to manager
  id = enemyMan->add(DE_ENEMY_MOVING_TYPE_TOWARDS_PLAYER, DE_ENEMY_VISUAL_TYPE_SQUARE, r, g, b, elementType);
  //set it's collision radius and hp
  enemyMan->setCollisionRadius(id, collRadius);
  enemyMan->setHP(id, hp);
  //set it position somewhere beyond the game screen, but now so far away
  switch (rand() % 4)
  {
  case 0:
    enemyMan->setPosition(id, rand() % 640, -40);
    break;
  case 1:
    enemyMan->setPosition(id, -40, rand() % 480);
    break;
  case 2:
    enemyMan->setPosition(id, 680, rand() % 480);
    break;
  case 3:
    enemyMan->setPosition(id, rand() % 640, 520);
    break;
  default:
    enemyMan->setPosition(id, rand() % 640, rand() % 480);
    break;
  }
  //set it's speed
  enemyMan->setSpeed(id, 20);
}

//on deny shooting
void deEventManager::onDenyShooting()
{
  //allow player shooting
  player->allowShooting();
}

//on deny element changing
void deEventManager::onDenyElementChanging()
{
  //allow player element changing
  player->allowElementChanging();
}