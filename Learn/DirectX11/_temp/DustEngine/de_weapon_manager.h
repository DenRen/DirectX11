#ifndef __DE_WEAPON_MANAGER_H
#define __DE_WEAPON_MANAGER_H

#include <vector>
#include "de_weapon.h"

//weapons
const int DE_WEAPON_SIMPLE_BLASTER = 0;
const int DE_WEAPON_SHOTGUN = 1;
const int DE_WEAPON_ASSAULT_RIFLE = 2;

//weapon manager class
class deWeaponManager
{
private:
  std::vector<deWeapon> vecWeapons; //vector of weapons
public:
  //initialize weapons
  void init(deProjectileManager *projMan);
  //return weapon by its id
  deWeapon *getWeapon(const int weaponId)
  {
    return &vecWeapons[weaponId];
  }
};

#endif