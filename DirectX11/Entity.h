#pragma once

#include "AnimatedSprite.h"

class Entity
{
public:
	Entity ();
	~Entity ();

	void InitializeSprite (ID3D11Device *device, Shader* shader, LPCSTR textureFileName, float spriteSize);
	void InitializeAnimatedSprite (ID3D11Device *device, ID3D11DeviceContext *deviceContext,
								   Shader *shader, LPCSTR textureFileName, float spriteSize,
								   float framesPerSecond, float animationSpeed = 1.0f, 
								   bool isLooping = true);

	void Update ();
	void Render (ID3D11DeviceContext *deviceContext,
				 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

	void SetPosition (float x  = 0.0f, float y = 0.0f);
	void SetVelocity (float Vx = 0.0f, float Vy = 0.0f);

private:
	D3DXMATRIX m_worldMatrix;
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_velocity;
	Sprite *m_sprite;
};

