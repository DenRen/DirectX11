#ifndef __DE_ENGINE_H
#define __DE_ENGINE_H

/*
  Main DustEngine module
*/

#include <windows.h>
#include <GL\gl.h>
#include "de_timer.h"
#include "de_event_manager.h"
#include "de_enemy_manager.h"
#include "de_projectile_manager.h"
#include "de_text.h"
#include "de_player.h"
#include "de_cursor.h"
#include "de_physics_manager.h"
#include "de_weapon_manager.h"
#include "de_texture_manager.h"

//include openGL lib
#pragma comment(lib, "opengl32")

//engine name and version
#define DE_ENGINE_NAME "DustEngine v0.1"

//game states
const int DE_GAME_STATE_GAME = 0;
const int DE_GAME_STATE_GAME_PAUSED = 1;
const int DE_GAME_STATE_GAME_MENU = 2;
const int DE_GAME_STATE_GAME_OVER = 3;
const int DE_GAME_STATE_MENU = 4;

//game engine class
class deEngine
{
private:
  //initialize render
  void initRender();
  deCursor cursor;             //game cursor
  deKeys keys;                 //pressed keys
  dePlayer player;             //player
  deTimer timer;               //HR timer
  deEventManager eventMan;     //event manager
  deEnemyManager enemyMan;     //enemy manager
  deProjectileManager projMan; //projectile manager
  deText textMan;              //text manager
  dePhysicsManager physMan;    //physics manager
  deWeaponManager weaponMan;   //weapon manager
  deTextureManager texMan;     //texture manager
  //display scene
  friend void display(deEngine *engine);
  //handle windows messages
  friend LRESULT CALLBACK winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
  int gameState; //game state
public:
  //init pointer at self
  deEngine();
  //create game window
  void createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd);
  //start engine
  void start();
  //main loop
  void mainLoop();
  //restart game
  void restartGame();
};

#endif