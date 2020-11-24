#ifndef __DE_EVENT_H
#define __DE_EVENT_H

//actions
const int DE_EVENT_ACTION_NONE = 0;                  //no action
const int DE_EVENT_ACTION_SPAWN_ENEMY = 1;           //spawn an enemy
const int DE_EVENT_ACTION_DENY_SHOOTING = 2;         //deny player shooting
const int DE_EVENT_ACTION_DENY_ELEMENT_CHANGING = 3; //deny player element changing
const int DE_EVENT_ACTION_SPAWN_MINIBOSS = 4;        //spawn a miniboss
const int DE_EVENT_ACTION_SPAWN_BOSS = 5;            //spawn a boss

//types
const int DE_EVENT_TYPE_SIMPLE = 0;          //executes action after passing the duration
const int DE_EVENT_TYPE_INFINITE = 1;        //like simple, but runs itself again after execution
const int DE_EVENT_TYPE_EVENT_CANCELLER = 2; //cancels event (should be run after infinite)

//basic event class
class deEvent
{
private:
  int action;         //event action
  int type;           //event type
  int id;             //unique identificator
  int data;           //data for some types of events
  double lastingTime; //how long it lasted
  double duration;    //it's duration
  bool completed;     //completed event flag
public:
  //create event based on given params
  deEvent(const int new_type, const int new_action,
    const double new_duration, const int new_data = 0)
  {
    //copy given params to new event object
    type = new_type;
    action = new_action;
    duration = new_duration;
    data = new_data;
    //set lasting time 0 and set it's not completed
    lastingTime = 0;
    completed = false;
  }
  //read access
  int getAction() {return action;}
  int getType() {return type;}
  int getId() {return id;}
  int getData() {return data;}
  double getLastingTime() {return lastingTime;}
  double getDuration() {return duration;}
  bool isCompleted() {return completed;}
  //write access
  void setId(const int new_id) {id = new_id;}
  //ancrease lasting time by given value
  void increaseTime(const double time) {lastingTime += time;}
  //start event again
  void startAgain() {lastingTime -= duration;}
  //flag it's as completed
  void completeEvent() {completed = true;}
};

#endif