#pragma once

#include "de_timer.h"
#include "Graphics.h"
#include "Shader.h"
#include "Texture.h"
#include "Rectangle.h"

struct SimpleVertex
{
	XMFLOAT3 pos;

	SimpleVertex () :
		pos (0, 0, 0)
	{}

	SimpleVertex (XMFLOAT3 pos) :
		pos (pos)
	{}

	static D3D11_INPUT_ELEMENT_DESC *GetLayout ()
	{
		static D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		return layout;
	}

	static int GetNumElements ()
	{
		return 1;
	};
};

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

	ID3D11Device *m_device;
	ID3D11DeviceContext *m_deviceContext;

	Graphics *m_graphics;
	static deTimer *m_timer;

	Shader *m_shader;
	Texture *m_texture;

	VertexBuffer <VertexPosTex, char> *m_vertexBuffer;
	RectTex *m_rect;

	ID3D11Buffer *m_CBMatrixes;
	//Camera *m_camera;

	ID3D11VertexShader *g_pVertexShader = nullptr;
	ID3D11PixelShader *g_pPixelShader	= nullptr;
	ID3D11InputLayout *g_pVertexLayout	= nullptr;
	ID3D11Buffer *g_pVertexBuffer		= nullptr;
};