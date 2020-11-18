#pragma once

#include "de_timer.h"
#include "Graphics.h"
#include "Shader.h"
#include "Texture.h"
#include "Rectangle.h"
#include "Camera.h"
#include "Structure_Data.h"
#include "Input.h"

class Engine
{
public:
	Engine ();
	~Engine ();

	bool InitializeGraphics (HWND hWnd, HINSTANCE hInstance);
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

	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;

	Graphics *m_graphics;
	static deTimer *m_timer;

	Shader *m_shader;
	Texture *m_texture;

	VertexBuffer <VertexPosTex, char> *m_vertexBuffer;
	RectTex *m_rect;

	Camera *m_camera;

	WVPMatrixes m_WVPMatrixes;
	ID3D11Buffer *m_CBWVPMatrixes;

	Input *m_input;
};