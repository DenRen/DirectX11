#pragma once

#include "Graphics.h"
#include "TextureShader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"

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
	TextureShader *shader;
	Texture *texture;

	// Temporary 2
	AnimatedSprite *m_animSprite;

	// Temporary 3
	ResourceManager *m_resourceManager;

	Sprite *m_sprite;
};

