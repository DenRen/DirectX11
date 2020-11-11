#pragma once

#include "Widget.h"
#include "Sprite.h"
#include "VertexBuffer.h"

class RectTex : public Widget, public Sprite <VertexPosTex>
{
public:
	RectTex (int coordX, int coordY, int width, int height,
			 Shader *shader, Texture *texture);

	void Initialize (int coordX, int coordY, int width, int height,
					 Shader *shader, Texture *texture);
	void Update ();
	void Draw ();

private:
	void SetVerticesXYWH (float coordX, float coordY, float width, float height);
	void SetDefaultTexture ();	
	void SetCoord (float newCoordX, float newCoordY);
	void SetIndexes ();
	void Move (float deltaX, float deltaY);

	VertexPosTex m_vert[4];
	unsigned m_indeces[6];

	ID3D11DeviceContext *m_deviceContext;
};

class Rectangle
{};
