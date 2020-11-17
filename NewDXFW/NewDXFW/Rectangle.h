#pragma once

#include "Sprite.h"
#include "VertexBuffer.h"

class RectTex : public Sprite <VertexPosTex, char>
{
public:
	RectTex (float coordX, float coordY, float width, float height,
			 Shader *shader, Texture *texture,
			 ID3D11Buffer *CBMatrixes, WVPMatrixes *WVPMatrixes);

	void Draw ();

private:
	/*void InitializeVB (float coorX, float coorY, float width, float height);

	void SetVerticesXYWH (float coordX, float coordY, float width, float height);
	void SetDefaultTexture ();
	void SetCoord (float newCoordX, float newCoordY);
	void SetIndexes ();
	void Move (float deltaX, float deltaY);*/

	XMMATRIX m_world;
	WVPMatrixes *m_WVPMatrixes;
};