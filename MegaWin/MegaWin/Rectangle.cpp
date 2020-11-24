#include "Rectangle.h"

RectTex::RectTex (ID3D11Device *device, ID3D11DeviceContext *deviceContext,
				  float coordX, float coordY, float width, float height,
				  Shader *shader, Texture *texture) :
	Sprite  (shader, texture),
	m_deviceContext (deviceContext)
{
	InitializeVB (device, coordX, coordY, width, height);
}

void RectTex::InitializeVB (ID3D11Device *device,
							float coordX, float coordY, float width, float height)
{
	// Init m_vert[]
	SetIndexes ();
	SetVerticesXYWH (coordX, coordY, width, height);
	SetDefaultTexture ();	// No animation!!! No sprites!!!

	//m_vertexBuffer.Initialize (device, m_vert, 4, m_indeces, 6);
	m_vertexBuffer.Initialize (device, m_vert, 8, m_indeces, 18);
}

void RectTex::Draw ()
{
	Sprite::Render (m_deviceContext);
}

void RectTex::SetVerticesXYWH (float coordX, float coordY, float width, float height)
{
	m_vert[0].pos = XMFLOAT3 (-0.0f, 1.0f, +0.0f);

	m_vert[1].pos = XMFLOAT3 (-0.5f, 0.0f, +0.5f);
	m_vert[2].pos = XMFLOAT3 (-0.5f, 0.0f, -0.5f);
	m_vert[3].pos = XMFLOAT3 (+0.5f, 0.0f, -0.5f);
	m_vert[4].pos = XMFLOAT3 (+0.5f, 0.0f, +0.5f);

		/*				XMFLOAT3 (+0.5f,  0.0f, +0.5f),
	m_vert[0].pos = XMFLOAT3 (coordX,		  coordY,		   -0.5f);
	m_vert[1].pos = XMFLOAT3 (coordX,		  coordY + height, -0.5f);
	m_vert[2].pos = XMFLOAT3 (coordX + width, coordY + height, -0.5f);
	m_vert[3].pos = XMFLOAT3 (coordX + width, coordY,		   -0.5f);*/
}

void RectTex::SetDefaultTexture ()
{
	m_vert[0].tex = XMFLOAT2 (0, 1);
	m_vert[1].tex = XMFLOAT2 (0, 0);
	m_vert[2].tex = XMFLOAT2 (1, 0);
	m_vert[3].tex = XMFLOAT2 (1, 1);
}

void RectTex::SetCoord (float newCoordX, float newCoordY)
{
	XMFLOAT3 oldCoord = m_vert[0].pos;
	float width  = m_vert[2].pos.x - oldCoord.x;
	float height = m_vert[2].pos.y - oldCoord.y;

	m_vert[0].pos = XMFLOAT3 (newCoordX,		 newCoordY,			 0);
	m_vert[1].pos = XMFLOAT3 (newCoordX,		 newCoordY + height, 0);
	m_vert[2].pos = XMFLOAT3 (newCoordX + width, newCoordY + height, 0);
	m_vert[3].pos = XMFLOAT3 (newCoordX + width, newCoordY,			 0);
}

void RectTex::SetIndexes ()
{
	char m_indeces_[18] = {
		0, 4, 1,
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,

		1, 4, 2,
		2, 3, 4
	};

	memcpy (m_indeces, m_indeces_, 18);
	/*
	m_indeces[0] = 0;
	m_indeces[1] = 1;
	m_indeces[2] = 2;

	m_indeces[3] = 0;
	m_indeces[4] = 2;
	m_indeces[5] = 3;*/
}

void RectTex::Move (float deltaX, float deltaY)
{
	XMFLOAT3 oldCoord = m_vert[0].pos;
	float width  = m_vert[2].pos.x - oldCoord.x;
	float height = m_vert[2].pos.y - oldCoord.y;
	float newCoordX = oldCoord.x + deltaX;
	float newCoordY = oldCoord.y + deltaY;

	m_vert[0].pos = XMFLOAT3 (newCoordX,		 newCoordY,			 0);
	m_vert[1].pos = XMFLOAT3 (newCoordX,		 newCoordY + height, 0);
	m_vert[2].pos = XMFLOAT3 (newCoordX + width, newCoordY + height, 0);
	m_vert[3].pos = XMFLOAT3 (newCoordX + width, newCoordY,			 0);
}