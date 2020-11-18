#pragma once

#include "Sprite.h"
#include "VertexBuffer.h"

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