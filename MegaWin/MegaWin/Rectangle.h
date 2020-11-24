#pragma once

#include "Widget.h"
#include "Sprite.h"
#include "VertexBuffer.h"

class RectTex : public Widget, private Sprite <VertexPosTex, char>
{
public:
	RectTex (ID3D11Device *device, ID3D11DeviceContext *deviceContext,
			 float coordX, float coordY, float width, float height,
			 Shader *shader, Texture *texture);

	void Draw ();

private:
	void InitializeVB (ID3D11Device *device,
					   float coordX, float coordY, float width, float height);

	void SetVerticesXYWH (float coordX, float coordY, float width, float height);
	void SetDefaultTexture ();	
	void SetCoord (float newCoordX, float newCoordY);
	void SetIndexes ();
	void Move (float deltaX, float deltaY);

	VertexPosTex m_vert[5];
	char m_indeces[18];

	ID3D11DeviceContext *m_deviceContext;
};