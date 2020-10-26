#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

class GameComponent
{
public:
	GameComponent () {}
	virtual ~GameComponent () {}

	virtual bool Initialize () = 0;
	virtual void Update () = 0;
	virtual void Render (ID3D11DeviceContext *deviceContext,
						 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix) = 0;
};

