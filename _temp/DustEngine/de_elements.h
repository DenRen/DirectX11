#ifndef __DE_ELEMENTS_H
#define __DE_ELEMENTS_H

//elements count
const int DE_ELEMENTS_COUNT = 5;

//element type
const int DE_ELEMENT_TYPE_NONE = -1;
const int DE_ELEMENT_TYPE_FIRE = 0;
const int DE_ELEMENT_TYPE_WATER = 1;
const int DE_ELEMENT_TYPE_AIR = 2;
const int DE_ELEMENT_TYPE_EARTH = 3;

//damage modifiers
const double DE_DAMAGE_MODIFIER_DOUBLE = 2.0;
const double DE_DAMAGE_MODIFIER_NO_MODIFIER = 1.0;
const double DE_DAMAGE_MODIFIER_HEAL = 0.5;

//get damage modifier
double elementalDamageModifier(const int attackerType, const int victimType);
//get element color
void getElementColor(const int element, double *r, double *g, double *b);
//get element type by index
int getElementTypeByIndex(const int index);

#endif