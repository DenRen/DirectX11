#pragma once

#include "de_timer.h"
#include "Graphics.h"
#include "Shader.h"
#include "Texture.h"
#include "Rectangle.h"

class Engine
{
public:
	Engine ();
	~Engine ();

	bool InitializeGraphics (HWND hWnd);
	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	void Run ();

	static Engine *GetEngine ();
	static double GetDeltaTime ();

	Graphics *GetGraphics ();

private:

	void Update ();
	void Render ();

	HWND m_hWnd;
	HINSTANCE m_hInstance;

	Graphics *m_graphics;
	static deTimer *m_timer;
	

	Shader *m_shader;
	Texture *m_texture;

	RectTex *m_rect;

	ID3D11Buffer *m_CBMatrixes;
	//Camera *m_camera;
	ID3D11Buffer *m_pVertexBuffer;
};