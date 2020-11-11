#include "Rectangle.h"

RectTex::RectTex (int coordX, int coordY, int width, int height,
				  Shader *shader, Texture *texture)
{
	auto vertexBuffer = new VertexBuffer <VertexPosTex> * 
	
	Initialize (coordX, coordY, width, height, shader, texture);
}

void RectTex::Initialize (int coordX, int coordY, int width, int height,
						  Shader *shader, Texture *texture)
{
	SetVerticesXYWH (coordX, coordY, width, height);
	SetDefaultTexture ();	// No animation!!! No sprites!!!
}

void RectTex::Update ()
{
	
}

void RectTex::Draw ()
{
	Sprite::Render (m_deviceContext);
}

void RectTex::SetVerticesXYWH (float coordX, float coordY, float width, float height)
{
	m_vert[0].pos = XMFLOAT3 (coordX,		  coordY,		   0);
	m_vert[1].pos = XMFLOAT3 (coordX,		  coordY + height, 0);
	m_vert[2].pos = XMFLOAT3 (coordX + width, coordY + height, 0);
	m_vert[3].pos = XMFLOAT3 (coordX + width, coordY,		   0);
}

void RectTex::SetDefaultTexture ()
{
	m_vert[0].uv = XMFLOAT2 (0, 1);
	m_vert[1].uv = XMFLOAT2 (0, 0);
	m_vert[2].uv = XMFLOAT2 (1, 0);
	m_vert[3].uv = XMFLOAT2 (1, 1);
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
	m_indeces[0] = 0;
	m_indeces[1] = 1;
	m_indeces[2] = 2;
	m_indeces[3] = 3;
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
