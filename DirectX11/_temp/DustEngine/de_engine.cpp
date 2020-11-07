#include "de_engine.h"

static HWND hWnd;
static HGLRC hGLRC;
static HDC hDC;
static deEngine *curEngine;

//init pointer at self
deEngine::deEngine()
{
  curEngine = this;
  gameState = DE_GAME_STATE_GAME; //and set game state
}

//display scene
void display(deEngine *engine)
{
  //clear scene
  glClear(GL_COLOR_BUFFER_BIT);
  //draw:
  //dead enemies
  engine->enemyMan.drawDead();
  //alive enemies
  engine->enemyMan.draw();
  //player
  engine->player.draw();
  //projectiles
  engine->projMan.draw();


  //-- info display begin --
  //current weapon

  //score
  char score[10];
  _itoa_s(engine->enemyMan.score(), score, 10, 10);
  if (engine->gameState == DE_GAME_STATE_GAME || engine->gameState == DE_GAME_STATE_GAME_PAUSED)
  {
    //elements
    char elementStr[80];
    strcpy_s(elementStr, 80, "Current Element:   Next Element:");
    engine->textMan.printText(10.0, 30.0, elementStr);

    //elements colors
    double r, g, b;
    double size = 10.0;
    //current element
    getElementColor(engine->player.getCurrentWeapon()->getElementType(), &r, &g, &b);
    //simple square
    glPushMatrix();
    glTranslatef((GLfloat)205, (GLfloat)22, 0.0);
    glColor3f((GLfloat)r, (GLfloat)g, (GLfloat)b);
    glRectf((GLfloat)size, (GLfloat)size, (GLfloat)-size, (GLfloat)-size);
    glPopMatrix();
    //next element
    getElementColor(engine->player.getCurrentWeapon()->getNextElementType(), &r, &g, &b);
    //simple square
    glPushMatrix();
    glTranslatef((GLfloat)385, (GLfloat)22, 0.0);
    glColor3f((GLfloat)r, (GLfloat)g, (GLfloat)b);
    glRectf((GLfloat)size, (GLfloat)size, (GLfloat)-size, (GLfloat)-size);
    glPopMatrix();

    //score
    char result[80];
    strcpy_s(result, 80, "Score: ");
    strcat_s(result, 80, score);
    engine->textMan.printText(10.0, 60.0, result);
  }
  if (engine->gameState == DE_GAME_STATE_GAME_OVER)
  {
    char result[80];
    strcpy_s(result, 80, "Game over! Your result: ");
    strcat_s(result, 80, score);
    engine->textMan.printText(170.0, 240.0, result);
    strcpy_s(result, 80, "Press R to restart...");
    engine->textMan.printText(205.0, 270.0, result);
  }
  //-- info display end --

  //cursor
  engine->cursor.draw();

  //wait for drawing to finish
  glFinish();
  //swap front and rear buffers
  SwapBuffers(wglGetCurrentDC());
}

//reshape window
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 640.0, 480.0, 0.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

//initialize render
void deEngine::initRender()
{
  //set clear color white
  glClearColor(1.0, 1.0, 1.0, 0.0);
  //and shade model flat
  glShadeModel(GL_FLAT);
}

//set window pixel format
bool SetWindowPixelFormat()
{
  int glPixelFormat;
  PIXELFORMATDESCRIPTOR pfd;

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cRedBits = 8;
  pfd.cRedShift = 16;
  pfd.cGreenBits = 8;
  pfd.cGreenShift = 8;
  pfd.cBlueBits = 8;
  pfd.cBlueShift = 0;
  pfd.cAlphaBits = 0;
  pfd.cAlphaShift = 0;
  pfd.cAccumBits = 64;
  pfd.cAccumRedBits = 16;
  pfd.cAccumGreenBits = 16;
  pfd.cAccumBlueBits = 16;
  pfd.cAccumAlphaBits = 0;
  pfd.cDepthBits = 32;
  pfd.cStencilBits = 8;
  pfd.cAuxBuffers = 0;
  pfd.iLayerType = PFD_MAIN_PLANE;
  pfd.bReserved = 0;
  pfd.dwLayerMask = 0;
  pfd.dwVisibleMask = 0;
  pfd.dwDamageMask = 0;
  glPixelFormat = ChoosePixelFormat(hDC, &pfd);
  if (!glPixelFormat)
  {
    glPixelFormat = 1;
    if (!DescribePixelFormat(hDC, glPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
      return false;
  }
  if (!SetPixelFormat(hDC, glPixelFormat, &pfd))
    return false;
  return true;
}

//handle windows messages
LRESULT CALLBACK winProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;

  switch (msg)
  {
  case WM_CREATE:
    //initialize on create
    hDC = GetDC(hWnd);
    if (!SetWindowPixelFormat())
      exit(EXIT_FAILURE);
    hGLRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hGLRC);
    break;
  case WM_DESTROY:
    //deinitialize on destroy
    if (hGLRC)
    {
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(hGLRC);
    }
    ReleaseDC(hWnd, hDC);
    PostQuitMessage(0);
    break;
  case WM_PAINT:
    //repaint on paint
    BeginPaint(hWnd, &ps);
    display(curEngine);
    EndPaint(hWnd, &ps);
    break;
  case WM_SIZE:
    //reshape() on resize
    reshape(LOWORD(lParam), HIWORD(lParam));
    break;
  case WM_LBUTTONDOWN:
    //"press" LMB
    curEngine->keys.downLMB();
    break;
  case WM_LBUTTONUP:
    //"unpress" LMB
    curEngine->keys.upLMB();
    break;
  case WM_RBUTTONDOWN:
    //"press" RMB
    curEngine->keys.downRMB();
    break;
  case WM_RBUTTONUP:
    //"unpress" RMB
    curEngine->keys.upRMB();
    break;
  case WM_MOUSEMOVE:
    //move cursor to new position
    curEngine->cursor.move(LOWORD(lParam), HIWORD(lParam));
    //if game state != game over, make player look at cursor
    if (curEngine->gameState == DE_GAME_STATE_GAME)
      curEngine->player.lookAt(&curEngine->cursor);
    break;
  case WM_KEYDOWN:
    //exit on ESC
    if (LOWORD(wParam) == 27)
      PostMessage(hWnd, WM_QUIT, 0, 0);
    if (toupper(LOWORD(wParam)) == 'P')
    {
      switch (curEngine->gameState)
      {
      case DE_GAME_STATE_GAME:
        curEngine->gameState = DE_GAME_STATE_GAME_PAUSED;
        break;
      case DE_GAME_STATE_GAME_PAUSED:
        curEngine->gameState = DE_GAME_STATE_GAME;
      }
    }
    if (toupper(LOWORD(wParam)) == 'R')
    {
      if (curEngine->gameState == DE_GAME_STATE_GAME_OVER)
      {
        curEngine->restartGame();
      }
    }
    //"press" key
    curEngine->keys.keyDown(toupper(LOWORD(wParam)));
    break;
  case WM_KEYUP:
    //"unpress" key
    curEngine->keys.keyUp(toupper(LOWORD(wParam)));
    break;
  default:
    //process other messages
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

//create game window
void deEngine::createWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  WNDCLASS wcl;
  //fill window class fields:
  //set given instance handle
  wcl.hInstance = hInstance;
  //set class name
  wcl.lpszClassName = "DustEngine32Class";
  //set our window proc
  wcl.lpfnWndProc = winProc;
  //set it's style
  wcl.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  //no icon
  wcl.hIcon = NULL;
  //load usual cursor
  wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
  //no menu
  wcl.lpszMenuName = NULL;
  //no extras
  wcl.cbClsExtra = 0;
  wcl.cbWndExtra = 0;
  //white background
  wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  //register class
  RegisterClass(&wcl);
  //compute window size and position
  int height, width, left, top;
  //640x480 + all the small things
  height = 480 + GetSystemMetrics(SM_CYCAPTION) + ((GetSystemMetrics(SM_CYBORDER) + GetSystemMetrics(SM_CXEDGE)) << 1) + 3;
  width = 640 + ((GetSystemMetrics(SM_CXBORDER) + GetSystemMetrics(SM_CXEDGE)) << 1) + 3;
  //position window at screen center
  left = (GetSystemMetrics(SM_CXSCREEN) - width) >> 1;
  top = (GetSystemMetrics(SM_CYSCREEN) - height) >> 1;
  //create window
  hWnd = CreateWindow("DustEngine32Class", DE_ENGINE_NAME,
    WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, left, top, width, height,
    HWND_DESKTOP, NULL, hInstance, NULL);
  //show it
  ShowWindow(hWnd, nShowCmd);
  //hide cursor
  ShowCursor(false);
  //update window
  UpdateWindow(hWnd);
}
 
//start engine
void deEngine::start()
{
  //initialize render
  initRender();
  //initialize font
  textMan.initFont();
  //start the timer
  timer.startTimer();
  //set player's projectile manager
  player.setProjectileManager(&projMan);
  //initialize physics manager
  physMan.init(&player, projMan.getListPointer(), enemyMan.getListPointer());
  //initialize weapons manager
  weaponMan.init(&projMan);
  //initialize event manager
  eventMan.setEnemyManager(&enemyMan);
  eventMan.setPlayer(&player);
}

//restart game
void deEngine::restartGame()
{
  gameState = DE_GAME_STATE_GAME;
  
  eventMan.stopAll();
  enemyMan.clear();
  projMan.clear();

  //testing stuff

  //spawn enemy every second
  int id;
  id = eventMan.addEvent(DE_EVENT_TYPE_INFINITE, DE_EVENT_ACTION_SPAWN_ENEMY, 1.5);
  id = eventMan.addEvent(DE_EVENT_TYPE_INFINITE, DE_EVENT_ACTION_SPAWN_MINIBOSS, 15.0);
  id = eventMan.addEvent(DE_EVENT_TYPE_INFINITE, DE_EVENT_ACTION_SPAWN_BOSS, 30.0);
  //eventMan.addEvent(DE_EVENT_TYPE_EVENT_CANCELLER, DE_EVENT_ACTION_NONE, 3.0, id);

  //player init
  
  player.setXY(320, 240);
  player.setAngle(0);
  player.setCollisionRadius(10);
  //give him the gun
  player.setWeapon(weaponMan.getWeapon(DE_WEAPON_SIMPLE_BLASTER));
  player.setHPMax(1);
  player.allowShooting();
  player.allowElementChanging();
}

//main loop
void deEngine::mainLoop()
{
  restartGame();

  //mainloop

  char fps_str[5] = "0"; //zero-term string containing current FPS
  char count_str[5];     //zero-term string containing various manager counts
  char caption[80];      //window caption
  MSG msg;               //message
  int fps = 0;           //current FPS
  double check = 0;      //time (for FPS counter)
  while (true)
  {
    //testing stuff
    if (player.getHP() <= 0)
      gameState = DE_GAME_STATE_GAME_OVER;
    //
    //process messages
    while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
      if (GetMessage(&msg, NULL, 0, 0))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else
        return;
    //update the timer
    timer.updateTimer();
    //if game state != game over
    if (gameState == DE_GAME_STATE_GAME)
    {
      //process events
      eventMan.updateEvents(timer.getTimeInterval());
      //change weapon
      if (keys.isPressed('1'))
        player.setWeapon(weaponMan.getWeapon(DE_WEAPON_SIMPLE_BLASTER));
      if (keys.isPressed('2'))
        player.setWeapon(weaponMan.getWeapon(DE_WEAPON_SHOTGUN));
      if (keys.isPressed('3'))
        player.setWeapon(weaponMan.getWeapon(DE_WEAPON_ASSAULT_RIFLE));
      //change element
      if (keys.isRMB() && player.isAllowedElementChanging())
      {
        //change elements
        player.getCurrentWeapon()->changeElements();
        //deny changing
        player.denyElementChanging();
        //allow after some time
        eventMan.addEvent(DE_EVENT_TYPE_SIMPLE, DE_EVENT_ACTION_DENY_ELEMENT_CHANGING, 0.5, 0);
      }
      //shoot if LMB is pressed and shooting is allowed
      if (keys.isLMB() && player.isAllowedShooting())
      {
        //shoot
        player.shootAt(&curEngine->cursor);
        //deny shooting
        player.denyShooting();
        //allow shooting after some time
        eventMan.addEvent(DE_EVENT_TYPE_SIMPLE, DE_EVENT_ACTION_DENY_SHOOTING,
          player.getCurrentWeapon()->getWaitingTime(), 0);
      }
      //move all the GOs
      physMan.moveAll(timer.getTimeInterval(), keys.getPlayerDirection());
      //make player face cursor
      player.lookAt(&cursor);
      //cleanup projectiles
      projMan.cleanup(0, 0, 640, 480);
      //process collisions
      physMan.processCollisions();
      //bury corpses
      enemyMan.buryTheDead();
    }
    //FPS counter
    //increase elapsed time and FPS
    check += timer.getTimeInterval();
    ++fps;
    //if one minute is elapsed - update FPS counter
    if (check >= 1)
    {
      _itoa_s(fps, fps_str, 5, 10);
      check = 0;
      fps = 0;
    }
    //update window caption:
    //engine name
    strcpy_s(caption, 80, DE_ENGINE_NAME);
    //+FPS
    strcat_s(caption, 80," [FPS=");
    strcat_s(caption, 80, fps_str);
    //+enemies count
    strcat_s(caption, 80, "][ENMS=");
    _itoa_s(enemyMan.count(), count_str, 5, 10);
    strcat_s(caption, 80, count_str);
    //+projectiles count
    strcat_s(caption, 80, "][PROJ=");
    _itoa_s(projMan.count(), count_str, 5, 10);
    strcat_s(caption, 80, count_str);
    //+collisions count
    strcat_s(caption, 80, "][COLL=");
    _itoa_s(physMan.collisionsCount(), count_str, 5, 10);
    strcat_s(caption, 80, count_str);
    //+corpses count
    strcat_s(caption, 80, "][CRPS=");
    _itoa_s(enemyMan.corpsesCount(), count_str, 5, 10);
    strcat_s(caption, 80, count_str);
    strcat_s(caption, 80, "]");
    //update caption
    SetWindowText(hWnd, caption);
    //display current frame
    display(this);
    //give time to other processes
    Sleep(15);
  }
}