#include "Entity.h"
#include "Timer.h"

Entity::Entity () :
	m_position (0, 0),
	m_velocity (0, 0),
	m_sprite (nullptr)
{
	D3DXMatrixIdentity (&m_worldMatrix);
}

Entity::~Entity ()
{
	delete m_sprite;
	m_sprite = nullptr;
}

void Entity::InitializeSprite (ID3D11Device * device, Shader * shader, LPCSTR textureFileName, float spriteSize)
{
	m_sprite = new Sprite (spriteSize);
	m_sprite->Initialize (device, shader, textureFileName, false);
}

void Entity::InitializeAnimatedSprite (ID3D11Device *device, ID3D11DeviceContext *deviceContext, Shader * shader, 
									   LPCSTR textureFileName, float spriteSize, float framesPerSecond, float animationSpeed, bool isLooping)
{
	m_sprite = new AnimatedSprite (spriteSize, framesPerSecond, animationSpeed, isLooping);
	((AnimatedSprite *) m_sprite)->Initialize (device, deviceContext, shader, textureFileName);
	
}

void Entity::Update ()
{
	double dt = Timer::GetDeltaTime ();

	m_position += m_velocity * dt;
	m_velocity *= 0.95f;

	D3DXMatrixTranslation (&m_worldMatrix, m_position.x, m_position.y, 0.0f);
	if (m_sprite != nullptr)
	{
		m_sprite->Update ();
	}
}

void Entity::Render (ID3D11DeviceContext * deviceContext, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
	if (m_sprite != nullptr)
	{
		m_sprite->Render (deviceContext, m_worldMatrix, viewMatrix, projectionMatrix);
	}
}

void Entity::SetPosition (float x, float y)
{
	m_position.x = x;
	m_position.y = y;
}

void Entity::SetVelocity (float Vx, float Vy)
{
	m_velocity.x = Vx;
	m_velocity.y = Vy;
}
