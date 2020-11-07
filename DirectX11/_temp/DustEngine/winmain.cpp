#include "de_engine.h"

/*
  Entry point module
*/

//entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  deEngine engine; //DustEngine class exemplar

  //create window
  engine.createWindow(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
  //start
  engine.start();
  //enter the matrix
  engine.mainLoop();
}