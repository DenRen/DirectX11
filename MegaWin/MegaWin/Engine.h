#pragma once

#include "Graphics.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "de_timer.h"
#include "Camera.h"
#include "Rectangle.h"

class Engine
{
public:
	Engine ();
	~Engine ();

	bool InitializeGraphics (HWND hWnd);
	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	void Run ();

	Graphics *GetGraphics ();

	static Engine *GetEngine ();
	static double getDeltaTime ();

private:

	void Update ();
	void Render ();

	Graphics *m_graphics;
	static deTimer *m_timer;

	Shader *m_shader;
	Texture *m_texture;

	RectTex *m_rect;

	Camera *m_camera;
};

