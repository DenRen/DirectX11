#ifndef __DE_ROUTINES_H
#define __DE_ROUTINES_H

//player directions
const int DE_DIRECTION_IDLE = 0;
const int DE_DIRECTION_N = 1;
const int DE_DIRECTION_NE = 2;
const int DE_DIRECTION_E = 3;
const int DE_DIRECTION_SE = 4;
const int DE_DIRECTION_S = 5;
const int DE_DIRECTION_SW = 6;
const int DE_DIRECTION_W = 7;
const int DE_DIRECTION_NW = 8;

//pi
#define DE_PI 3.1415926535

const double DE_MELEE_ATTACK_RANGE = 2.0; //melee attack range

//convert degrees to radians
double deg2rad(const double deg);

#endif