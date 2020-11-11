#include "AnimatedSprite.h"
#include "AddFunc.h"
#include "Timer.h"
#include "Engine.h"

AnimatedSprite::AnimatedSprite (float size, float framesPerSecond, float animationSpeed, bool isLooping) :
	Sprite (size),
	m_framesPerSecond (1.0f / framesPerSecond),
	m_animationSpeed (animationSpeed),
	m_isLooping (isLooping),
	m_currentFrame (0),
	m_currentSpeed (0)
{}

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
	//m_maxFrames = (float) m_texture->GetWidth () / (float) (m_texture->GetHeight ());
	m_maxFrames = 5;
	return true;
}

void AnimatedSprite::Update ()
{
	if (m_maxFrames == 1.0f)
	{
		return;
	}

	if (m_currentFrame < m_maxFrames)
	{
		//float dt = Timer::GetDeltaTime ();
		double dt = Engine::getDeltaTime ();

		m_currentSpeed += m_animationSpeed * dt;
		//printf ("%f\n", m_currentSpeed);

		if (m_currentSpeed >= m_framesPerSecond)
		{
			m_currentFrame++;
			m_currentSpeed = 0.0f;

			if (m_currentFrame >= m_maxFrames)
			{
				if (m_isLooping)
				{
					m_currentFrame = 0.0f;
				}
				else
				{
					m_currentFrame = m_maxFrames;
				}
			}
		}
	}

	// No reaseon to lock/update the buffer if its still the same frame
	if (m_currentFrame == m_previousFrame)
	{
		return;
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexBuffer::VertexType *vertices = m_vertexBuffer->GetVertices ();

	const int _frame = 0;

	vertices[0].uv.x = m_currentFrame / m_maxFrames;
	vertices[0].uv.y = _frame * 0.5 + 0.5f;

	vertices[1].uv.x = m_currentFrame / m_maxFrames;
	vertices[1].uv.y = _frame * 0.5;

	vertices[2].uv.x = (m_currentFrame + 1.0f) / m_maxFrames;
	vertices[2].uv.y = _frame * 0.5;

	vertices[3].uv.x = (m_currentFrame + 1.0f) / m_maxFrames;
	vertices[3].uv.y = _frame * 0.5 + 0.5f;

	// Lock the vertexbuffer so it written to
	HRESULT result = m_deviceContext->Map (m_vertexBuffer->GetVertexBuffer (), 0, 
										   D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED (result))
	{
		return;
	}

	VertexBuffer::VertexType *verticesPtr = (VertexBuffer::VertexType *) mappedResource.pData;
	memcpy (verticesPtr, (void *) vertices, sizeof (*vertices) * m_vertexBuffer->GetVertexCount ());

	m_deviceContext->Unmap (m_vertexBuffer->GetVertexBuffer(), 0);

	// Update previouse frame
	m_previousFrame = m_currentFrame;
}

void AnimatedSprite::Render (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	Engine::GetEngine ()->GetGraphics ()->EnableAlphaBlending (true);
	Sprite::Render (deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	Engine::GetEngine ()->GetGraphics ()->EnableAlphaBlending (false);
}
