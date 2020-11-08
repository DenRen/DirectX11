#pragma once

#include "GameComponent.h"
#include "Player.h"

class GameScene : public GameComponent
{
public:
	GameScene ();
	virtual ~GameScene ();

	bool Initialize ();
	void Update ();
	void Render (ID3D11DeviceContext *deviceContext, 
				 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

private:
	Player *m_player;
};

