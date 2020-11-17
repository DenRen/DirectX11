#include "Rectangle.h"
#include "DXManager.h"

RectTex::RectTex (float coordX, float coordY, float width, float height,
				  Shader *shader, Texture *texture, ID3D11Buffer *CBMatrix,
				  WVPMatrixes *WVPMatrixes) :

	Sprite (shader, texture, CBMatrix, WVPMatrixes)
{
	VertexPosTex vert[4] = {{XMFLOAT3 (-0.5f, -0.5f, 0.0f), XMFLOAT2 (0.0f, 1.0f)},
							{XMFLOAT3 (-0.5f,  0.5f, 0.0f), XMFLOAT2 (0.0f, 0.0f)}, 
							{XMFLOAT3 ( 0.5f,  0.5f, 0.0f), XMFLOAT2 (1.0f, 0.0f)}, 
							{XMFLOAT3 ( 0.5f, -0.5f, 0.0f), XMFLOAT2 (1.0f, 1.0f)}};
	
	char indeces[6] = {0, 1, 2,
					   0, 2, 3};

	m_vertexBuffer.Initialize (DXManager::GetDevice (), vert, 4, indeces, 6);
}

void RectTex::Draw ()
{
	Sprite::Render (DXManager::GetDeviceContext ());
}