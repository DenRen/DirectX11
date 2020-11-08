#include "de_timer.h"

//initialize timer
void deTimer::init()
{
  //get frequency and calculate tick length
  frequency = getFrequency();
  tickLength = 1.0 / frequency;
}

//returns timer frequency
double deTimer::getFrequency()
{
  LARGE_INTEGER freq;

  if (!QueryPerformanceFrequency(&freq))
    return 0;
  
  return (double) freq.QuadPart;
}

//returns time
double deTimer::readTimer()
{
  //use only 1st processor core
  DWORD_PTR oldMask = SetThreadAffinityMask(GetCurrentThread(), 0);
  //read current time
  LARGE_INTEGER currentTime;
  QueryPerformanceCounter(&currentTime);
  //restore core using
  SetThreadAffinityMask(GetCurrentThread(), oldMask);
  //return current time (time elapsed from start)
  return (currentTime.QuadPart - start.QuadPart) * tickLength;
}

//start timer
void deTimer::startTimer()
{
  //initialize timer
  init();
  //use only 1st processor core
  DWORD_PTR oldMask = SetThreadAffinityMask(GetCurrentThread(), 0);
  //read current time
  QueryPerformanceCounter(&start);
  //restore core using
  SetThreadAffinityMask(GetCurrentThread(), oldMask);
  //init current and last check time
  lastCheckTime = currentCheckTime = readTimer();
  //init randomizer
  srand((int)start.QuadPart);
}

//update timer
void deTimer::updateTimer()
{
  //current time check is last now
  lastCheckTime = currentCheckTime;
  //read current time
  currentCheckTime = readTimer();
}