#ifndef __DE_GAME_OBJECT_H
#define __DE_GAME_OBJECT_H

#include <math.h>
#include "de_routines.h"

//basic gameobject class, abstract
class deGameObject
{
protected:
  int elementType;   //its element
  double x, y;       //coordinates of object CENTER
  double oldX, oldY; //backup of x and y
  double angle;      //orientation (0 = EAST!)
  double collRadius; //radius of collision circle
  int hpMax;         //maximum hp
  int hp;            //current hp
  double speed;      //moving speed
  double accel;      //moving acceleration
  int id;            //object id
  bool dead;         //dead GO flag
public:
  //add some fault-safe values
  deGameObject()
  {
    x = y = oldX = oldY = angle = collRadius = 0;
    hpMax = hp = 1;
    dead = false;
  }
  //set x and y
  void setXY(const double new_x, const double new_y)
  {
    x = new_x;
    y = new_y;
  }
  //position backup and restore
  void backupPosition() {oldX = x; oldY = y;}
  void restorePosition() {x = oldX; y = oldY;}
  //read access
  deGameObject *self() {return this;}
  double getX() {return x;}
  double getY() {return y;}
  double getCollisionRadius() {return collRadius;}
  int getHPMax() {return hpMax;}
  int getHP() {return hp;}
  int getId() {return id;}
  int getElementType() {return elementType;}
  //write access
  void setId(const int new_id) {id = new_id;}
  void setAngle(const double new_angle) {angle = new_angle;}
  void setSpeed(const double new_speed) {speed = new_speed;}
  void setAccel(const double new_accel) {accel = new_accel;}
  void setCollisionRadius(const double new_collRadius) {collRadius = new_collRadius;}
  //set maximum and current hp
  void setHPMax(const int new_hpMax)
  {
    hpMax = new_hpMax;
    hp = hpMax;
  }
  //angle functions:
  //calculate angle with which this GO will be facing given GO
  double getAngleAt(const deGameObject *obj)
  {
    double result;

    result = atan((y - obj->y) / (x - obj->x)) / DE_PI * 180;
    if (x > obj->x)
      result += 180;
    return result;
  }
  //calculate angle with which this GO will be facing point described by it''s coordinates
  double getAngleAt(const double targetX, const double targetY)
  {
    double result;

    result = atan((y - targetY) / (x - targetX)) / DE_PI * 180;
    if (x > targetX)
      result += 180;
    return result;
  }
  //rotate GO to face other GO
  void lookAt(const deGameObject *obj)
  {
    angle = getAngleAt(obj);
  }
  //rotate GO to face point point described by it''s coordinates
  void lookAt(const double targetX, const double targetY)
  {
    angle = getAngleAt(targetX, targetY);
  }
  //calculate distance between this and given GO
  double distance(deGameObject *obj)
  {
    return sqrt(pow(x - obj->getX(), 2) + pow(y - obj->getY(), 2));
  }
  //virtual funcs
  virtual void draw() = 0;
  virtual void hit(const int damage) {};
  virtual void heal(const int value) {};
  virtual ~deGameObject() {};
};

#endif