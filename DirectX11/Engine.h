#pragma once

#include "Graphics.h"
#include "TextureShader.h"
#include "Texture.h"
#include "VertexBuffer.h"

class Engine
{
public:
	~Engine ();

	bool InitializeGraphics (HWND hWnd);
	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	void Run ();
	
	void Release ();

	Graphics *GetGraphics ();
	static Engine *GetEngine ();

private:
	Engine ();

	void Update ();
	void Render ();

	Graphics *m_graphics;
	static Engine *m_Instance;

	// Temporary
	VertexBuffer *vertexBuffer;
	TextureShader *textureShader;
	Texture *texture;
};

