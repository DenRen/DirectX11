#pragma once
#include "Sprite.h"

class AnimatedSprite : public Sprite
{
public:
	AnimatedSprite (float size, float framesPerSecond, float animationSpeed = 1.0f,
					bool isLooping = true);
	~AnimatedSprite ();

	bool Initialize (ID3D11Device *device, ID3D11DeviceContext *deviceContext, 
					 Shader *shader, LPCTSTR textureFileName);
	void Update () {}
	void Render (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix,
						 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

private:
	ID3D11DeviceContext *m_deviceContext;
	float m_currentFrame;
	float m_previousFrame;
	float m_maxFrames;
	float m_animationSpeed;
	float m_currentSpeed;
	float m_framesPerSecond;
	bool m_isLooping;
};

