#include "de_projectile_manager.h"

int deProjectileManager::idGenerator; //generator of ids

//add new projectile
int deProjectileManager::add(const int new_movingType, const int new_visualType,
  const double new_r, const double new_g, const double new_b, const int new_elementType)
{
  //create projectile and generate an id
  deProjectile proj(new_movingType, new_visualType, new_r, new_g, new_b, new_elementType);
  proj.setId(idGenerator++);
  //put it to list
  lstProjectiles.push_back(proj);
  //return it's id
  return proj.getId();
}

//various write funcs:
//method: find projectile with given id and modify the param
void deProjectileManager::setPosition(const int id, const double new_x, const double new_y)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setXY(new_x, new_y);
      return;
    }
}

void deProjectileManager::setAngle(const int id, const double new_angle)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setAngle(new_angle);
      return;
    }
}

void deProjectileManager::setCollisionRadius(const int id, const double new_collRadius)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setCollisionRadius(new_collRadius);
      return;
    }
}

void deProjectileManager::setSpeed(const int id, const double new_speed)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setSpeed(new_speed);
      return;
    }
}

void deProjectileManager::setAccel(const int id, const double new_accel)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setAccel(new_accel);
      return;
    }
}

void deProjectileManager::setTargetType(const int id, const int new_targetType)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setTargetType(new_targetType);
      return;
    }
}

void deProjectileManager::setDamage(const int id, const int new_damage)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setDamage(new_damage);
      return;
    }
}

void deProjectileManager::setPenetrations(const int id, const int new_penetrations)
{
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
    if (i->getId() == id)
    {
      i->setPenetrations(new_penetrations);
      return;
    }
}

//delete projectiles beyond the game field and with penetrations = 0
void deProjectileManager::cleanup(const int left, const int top, const int width, const int height)
{
  //mark projectiles for cleaning up
  for (std::list<deProjectile>::iterator i = lstProjectiles.begin(); i != lstProjectiles.end(); ++i)
  {
    //beyond the game field
    if ((i->getX() >= width + 10) || (i->getX() <= left - 10) || (i->getY() >= height + 10) || (i->getY() <= top - 10))
      i->toBeDeleted();
    //with penetrations = 0
    if (i->getPenetrations() == 0)
      i->toBeDeleted();
  }
  //delete them
  std::list<deProjectile>::iterator i = lstProjectiles.begin();
  while (i != lstProjectiles.end())
  {
    if (i->isToBeDeleted())
    {
      lstProjectiles.erase(i);
      i = lstProjectiles.begin();
    }
    else
      ++i;
  }
}