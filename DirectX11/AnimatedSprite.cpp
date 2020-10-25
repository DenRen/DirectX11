#include "AnimatedSprite.h"
#include "AddFunc.h"

AnimatedSprite::AnimatedSprite (float size, float framesPerSecond, float animationSpeed, bool isLooping) :
	Sprite (size),
	m_framesPerSecond (1000.0f / framesPerSecond / 1000.0f),
	m_animationSpeed (animationSpeed),
	m_isLooping (isLooping),
	m_currentFrame (0)
{

}

AnimatedSprite::~AnimatedSprite ()
{

}

bool AnimatedSprite::Initialize (ID3D11Device *device, ID3D11DeviceContext *deviceContext,
								 Shader *shader, LPCTSTR textureFileName)
{
	bool result = Sprite::Initialize (device, shader, textureFileName, true);
	if (!result)
	{
		RETURN_FALSE;
	}


	m_deviceContext = deviceContext;
	m_currentFrame = 0.0f;
	m_previousFrame = -1.0f;
	m_maxFrames = (float) m_texture->GetWidth () / (float) (m_texture->GetHeight ());

	return true;
}

void AnimatedSprite::Render (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (m_maxFrames == 1.0f)
	{
		return;
	}

	if (m_currentFrame < m_maxFrames)
	{
		
	}
}
