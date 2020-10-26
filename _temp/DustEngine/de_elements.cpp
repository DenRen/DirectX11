#include "de_elements.h"

//get damage modifier
double elementalDamageModifier(const int attackerType, const int victimType)
{
  //no modifier if no element
  if (attackerType == DE_ELEMENT_TYPE_NONE)
    return DE_DAMAGE_MODIFIER_NO_MODIFIER;
  //heal if elements are equal
  if (attackerType == victimType)
    return DE_DAMAGE_MODIFIER_HEAL;

  switch (attackerType)
  {
    //fire hits:
  case DE_ELEMENT_TYPE_FIRE:
    switch (victimType)
    {
      //water
    case DE_ELEMENT_TYPE_WATER:
      return DE_DAMAGE_MODIFIER_DOUBLE;
      //other
    default:
      return DE_DAMAGE_MODIFIER_NO_MODIFIER;
    }
    break;
    //water hits:
  case DE_ELEMENT_TYPE_WATER:
    switch (victimType)
    {
      //fire
    case DE_ELEMENT_TYPE_FIRE:
      return DE_DAMAGE_MODIFIER_DOUBLE;
      //other
    default:
      return DE_DAMAGE_MODIFIER_NO_MODIFIER;
    }
    break;
    //air hits:
  case DE_ELEMENT_TYPE_AIR:
    switch (victimType)
    {
      //earth
    case DE_ELEMENT_TYPE_EARTH:
      return DE_DAMAGE_MODIFIER_DOUBLE;
      //other
    default:
      return DE_DAMAGE_MODIFIER_NO_MODIFIER;
    }
    break;
    //earth hits:
  case DE_ELEMENT_TYPE_EARTH:
    switch (victimType)
    {
      //air
    case DE_ELEMENT_TYPE_AIR:
      return DE_DAMAGE_MODIFIER_DOUBLE;
      //other
    default:
      return DE_DAMAGE_MODIFIER_NO_MODIFIER;
    }
    break;
  default:
    return DE_DAMAGE_MODIFIER_NO_MODIFIER;
  }
}

//get element color
void getElementColor(const int element, double *r, double *g, double *b)
{
  switch (element)
  {
  case DE_ELEMENT_TYPE_FIRE:
    *r = 0.9;
    *g = 0.15;
    *b = 0.0;
    break;
  case DE_ELEMENT_TYPE_WATER:
    *r = 0.0;
    *g = 0.3;
    *b = 0.75;
    break;
  case DE_ELEMENT_TYPE_AIR:
    *r = 0.6;
    *g = 1.0;
    *b = 0.93;
    break;
  case DE_ELEMENT_TYPE_EARTH:
    *r = 0.33;
    *g = 0.16;
    *b = 0.0;
    break;
  default:
    *r = 0.7;
    *g = 0.7;
    *b = 0.7;
    break;
  }
}

//get element type by index
int getElementTypeByIndex(const int index)
{
  switch (index)
  {
  case 0:
    return DE_ELEMENT_TYPE_FIRE;
  case 1:
    return DE_ELEMENT_TYPE_WATER;
  case 2:
    return DE_ELEMENT_TYPE_AIR;
  case 3:
    return DE_ELEMENT_TYPE_EARTH;
  default:
    return DE_ELEMENT_TYPE_NONE;
  }
}