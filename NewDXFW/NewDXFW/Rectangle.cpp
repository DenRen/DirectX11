#include "Rectangle.h"
#include "DXManager.h"
#include "ResourceManager.h"

Texture		 *RectTex::defTexture	  = nullptr;
Shader		 *RectTex::defShader	  = nullptr;
ID3D11Buffer *RectTex::defCBMatrixes  = nullptr;
WVPMatrixes  *RectTex::defWVPMatrixes = nullptr;

RectTex::RectTex (float coorX, float coorY, float width, float height) :
	RectTex (coorX, coorY, width, height,
			 defTexture, defShader,
			 defCBMatrixes, defWVPMatrixes)
{}

RectTex::RectTex (float coorX, float coorY, float width, float height,
				  const char *pathTexture) :
	RectTex (coorX, coorY, width, height,
			 ResMgr::GetResMgr ()->GetTexture (pathTexture), defShader,
			 defCBMatrixes, defWVPMatrixes)
{}

RectTex::RectTex (float coorX, float coorY, float width, float height,
				  Texture *texture, Shader *shader,
				  ID3D11Buffer *CBMatrix, WVPMatrixes *WVPMatrixes) :
	Sprite (shader, texture, CBMatrix, WVPMatrixes)
{
	InitializeVB (coorX, coorY, width, height);

	ScaleUp (width, height, 1.0f);
	Move (coorX + width / 2, coorY - height / 2, 0.0f);
}

void RectTex::Draw ()
{
	Sprite::Render (DXManager::GetDeviceContext ());
}

void RectTex::SetDefaultValue (Texture *texture, Shader *shader,
							   ID3D11Buffer *CBMatrixes, WVPMatrixes *WVPMatrixes)
{
	defTexture = texture;
	defShader = shader;
	defCBMatrixes = CBMatrixes;
	defWVPMatrixes = WVPMatrixes;
}

void RectTex::InitializeVB (float coorX, float coorY, float width, float height)
{
	VertexPosTex vert[4] = {{XMFLOAT3 (-0.5f, -0.5f, 0.0f), XMFLOAT2 (0.0f, 1.0f)},
							{XMFLOAT3 (-0.5f,  0.5f, 0.0f), XMFLOAT2 (0.0f, 0.0f)},
							{XMFLOAT3 ( 0.5f,  0.5f, 0.0f), XMFLOAT2 (1.0f, 0.0f)},
							{XMFLOAT3 ( 0.5f, -0.5f, 0.0f), XMFLOAT2 (1.0f, 1.0f)} };

	char indeces[6] = {0, 1, 2,
					   0, 2, 3 };

	m_vertexBuffer.Initialize (DXManager::GetDevice (), vert, 4, indeces, 6);
}
