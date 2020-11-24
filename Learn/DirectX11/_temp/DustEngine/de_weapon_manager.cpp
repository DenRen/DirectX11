#include "de_weapon_manager.h"

//initialize weapons
void deWeaponManager::init(deProjectileManager *projMan)
{
  //simple blaster
  vecWeapons.push_back(deWeapon(DE_WEAPON_SIMPLE_BLASTER, DE_DAMAGE_TYPE_CONSTANT, DE_ELEMENT_TYPE_NONE, 10, 10, 5.0,
    200.0, 200.0, 0.0, DE_INFINITE_PENETRATIONS, 1, 0.2, 0.7, 0.7, 0.7,
    DE_PROJECTILE_MOVING_TYPE_STRAIGHT, DE_TARGET_TYPE_ENEMY));
  vecWeapons.back().setProjectileManager(projMan);
  //shotgun
  vecWeapons.push_back(deWeapon(DE_WEAPON_SHOTGUN, DE_DAMAGE_TYPE_CONSTANT, DE_ELEMENT_TYPE_NONE, 7, 7, 3.0,
    500.0, 300.0, 15, 1, 10, 0.8, 0.7, 0.7, 0.7,
    DE_PROJECTILE_MOVING_TYPE_STRAIGHT, DE_TARGET_TYPE_ENEMY));
  vecWeapons.back().setProjectileManager(projMan);
  //assault rifle
  vecWeapons.push_back(deWeapon(DE_WEAPON_ASSAULT_RIFLE, DE_DAMAGE_TYPE_CONSTANT, DE_ELEMENT_TYPE_NONE, 15, 15, 2.0,
    500.0, 300.0, 5, 1, 1, 0.25, 0.7, 0.7, 0.7,
    DE_PROJECTILE_MOVING_TYPE_STRAIGHT, DE_TARGET_TYPE_ENEMY));
  vecWeapons.back().setProjectileManager(projMan);
}