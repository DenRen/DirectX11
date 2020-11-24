#pragma once

#include "Sprite.h"
#include "VertexBuffer.h"
#include "NewsQueue.h"

struct RectFigure
{
	RectFigure ();
	RectFigure (float coorX, float coorY, float width, float height);

	bool IsContain (MousePosition mousePosition);
	bool IsContain (float coorX, float coorY);

	void ScaleUp (float scaleX, float scaleY);
	void Move (float deltaX, float deltaY);

	void Transpose ();

	float m_coorX;
	float m_coorY;
	float m_width;
	float m_height;
};

class RectTex : public Sprite <VertexPosTex, char>
{
public:
	RectTex (float coorX, float coorY, float width, float height);
	RectTex (float coorX, float coorY, float width, float height,
			 const char *pathTexture);
	RectTex (float coorX, float coorY, float width, float height,
			 Texture *texture, Shader *shader,
			 ID3D11Buffer *CBMatrixes, WVPMatrixes *WVPMatrixes);

	void Draw ();
	static void SetDefaultValue (Texture *texture, Shader *shader,
								 ID3D11Buffer *CBMatrixes, WVPMatrixes *WVPMatrixes);

	void ScaleUp (float ScaleUpX, float ScaleUpY, float ScaleUpZ);
	void Move (float x, float y, float z);

	bool CheckContainCursor (float coorX, float coorY);

	RectFigure GetRectFigure ();

private:

	void InitializeVB (float coorX, float coorY, float width, float height);

	RectFigure m_rectFigure;

	static Texture		*defTexture;
	static Shader		*defShader;
	static ID3D11Buffer	*defCBMatrixes;
	static WVPMatrixes  *defWVPMatrixes;
};