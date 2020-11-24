#ifndef __DE_TIMER_H
#define __DE_TIMER_H

#include <windows.h>

//HR timer class
class deTimer
{
  LARGE_INTEGER start;     //start time
  double frequency;        //timer frequency
  double tickLength;       //tick length
  double currentCheckTime; //current time
  double lastCheckTime;    //last checked time
  //initialize timer
  void init();
  //returns timer frequency
  double getFrequency();
  //returns time
  double readTimer();
public:
  //start timer
  void startTimer();
  //update timer
  void updateTimer();
  //get time elapsed from last check
  double getTimeInterval() {return currentCheckTime - lastCheckTime;}
};

#endif