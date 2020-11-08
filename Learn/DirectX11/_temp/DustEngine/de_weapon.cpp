#include "de_weapon.h"

//shoot at direction of point described by it's coordinates
void deWeapon::shoot(const double angle, const double x, const double y)
{
  //calculate damage
  int damage;
  switch (damageType)
  {
  case DE_DAMAGE_TYPE_CONSTANT:
    damage = damageMin;
    break;
  }

  //add projectiles
  for (int i = 0; i < numberOfProjectiles; ++i)
  {
    int id;
    id = projMan->add(projMovingType, DE_PROJECTILE_VISUAL_TYPE_SQUARE, r, g, b, elementType);
    projMan->setPosition(id, x, y);
    //angle
    double deflection;
    deflection = (float)(rand() % 101) / 100 * maxDeflection;
    if (rand() % 2 == 1)
      deflection = -deflection;
    double projAngle = angle + deflection;
    projMan->setAngle(id, projAngle);
    //
    projMan->setCollisionRadius(id, collisionRadius);
    projMan->setTargetType(id, targetType);
    projMan->setSpeed(id, speed);
    projMan->setAccel(id, accel);
    projMan->setDamage(id, damage);
    projMan->setPenetrations(id, penetrateLevel);
  }
}