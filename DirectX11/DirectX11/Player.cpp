#include "Player.h"

Player::Player () :
	m_entity (nullptr)
{}

Player::~Player ()
{}

void Player::Initialize (ID3D11Device *device, ID3D11DeviceContext *deviceContext, Shader *shader)
{
	m_entity = EntityManager::GetInstance ()->AddEntity ();
	m_entity->InitializeAnimatedSprite (device, deviceContext, shader, "Texture\\ninja.png", 300.0f, 25.0f);
}

void Player::Update ()
{
	UpdateInput ();
}

void Player::UpdateInput ()
{
	m_entity->SetVelocity (0, 0);

	Input *input = Engine::GetEngine ()->GetInput ();
	if (input == nullptr)
	{
		return;
	}

	if (input->IsKeyDown (DIK_D))
	{
		m_entity->SetVelocity (300.0f, 0);
	}

	if (input->IsKeyDown (DIK_A))
	{
		m_entity->SetVelocity (-300.0f, 0);
	}

	if (input->IsKeyDown (DIK_ESCAPE))
	{
		PostQuitMessage (0);
		DestroyWindow (Engine::GetEngine ()->GetGraphics ()->GethWnd ());
	}
}
