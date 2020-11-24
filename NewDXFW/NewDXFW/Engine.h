#pragma once

#include "de_timer.h"
#include "Graphics.h"
#include "Shader.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Camera.h"
#include "Structure_Data.h"
#include "InputDX.h"
#include "ResourceManager.h"
#include "WindowManager.h"
#include "Text.h"

class Engine
{
public:
	Engine ();
	~Engine ();
	
	bool IntializePrelaunchParams ();
	bool InitializeGraphics (HWND hWnd, HINSTANCE hInstance);
	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	void Run ();

	static Engine *GetEngine ();
	static double GetDeltaTime ();

	Graphics *GetGraphics ();
	
private:

	void LoadResources ();
	bool InitializeCamera ();

	void Update ();
	void Render ();

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;

	Graphics *m_graphics;
	static deTimer *m_timer;

	Shader *m_shader;
	Texture *m_texture;

	VertexBuffer <VertexPosTex, char> *m_vertexBuffer;
	RectTex *m_rect;
	RectTex *m_rect1;

	Camera *m_camera;

	WVPMatrixes m_WVPMatrixes;
	ID3D11Buffer *m_CBWVPMatrixes;

	InputDX *m_input;

	WindowManager *m_windowManager;
	
	//-------------------------------------

	BitmapFont *m_font;
	Text *text1;
	Text *text2;
	Text *text3;
};