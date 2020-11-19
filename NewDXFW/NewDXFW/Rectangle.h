#pragma once

#include "Sprite.h"
#include "VertexBuffer.h"

struct RectFigure
{
	RectFigure () :
		RectFigure (0, 0, 0, 0)
	{}
	RectFigure (float coorX, float coorY, float width, float height) :
		m_coorX (coorX), m_coorY (coorY), m_width (width), m_height (height)
	{}

	void Scale (float scaleX, float scaleY);
	void Move (float deltaX, float deltaY);

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

private:
	void InitializeVB (float coorX, float coorY, float width, float height);

	static Texture		*defTexture;
	static Shader		*defShader;
	static ID3D11Buffer	*defCBMatrixes;
	static WVPMatrixes  *defWVPMatrixes;
};