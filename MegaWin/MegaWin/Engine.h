#pragma once

#include "Graphics.h"
#include "TextureShader.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "de_timer.h"
#include "EntityManager.h"
#include "Input.h"
#include "GameComponent.h"

class Engine
{
public:
	Engine ();
	~Engine ();

	bool InitializeGraphics (HWND hWnd);
	bool Initialize (HINSTANCE hInstance, HWND hWnd);
	void Run ();
	
	void Release ();

	void SetGameComponent (GameComponent * gameComponent);

	Graphics *GetGraphics ();
	Input *GetInput ();

	static Engine *GetEngine ();
	static double getDeltaTime ();

private:

	void Update ();
	void Render ();

	Graphics *m_graphics;
	static Engine *m_Instance;
	
	static deTimer *m_timer;

	// Temporary
	VertexBuffer *vertexBuffer;
	TextureShader *shader;
	Texture *texture;

	// Temporary 2
	AnimatedSprite *m_animSprite;

	// Temporary 3
	//ResourceManager *m_resourceManager;
	
	Sprite *m_sprite;

	// Temporary 4
	ResourceManager *m_resourceManager;
	Entity *m_entity;

	// Temporary 5

	EntityManager *m_entityManager;

	Input *m_input;

	GameComponent *m_gameComponent;

};

