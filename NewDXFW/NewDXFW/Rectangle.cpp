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

void RectTex::ScaleUp (float ScaleUpX, float ScaleUpY, float ScaleUpZ)
{
	Sprite::ScaleUp (ScaleUpX, ScaleUpY, ScaleUpZ);
	m_rectFigure.ScaleUp (ScaleUpX, ScaleUpY);
}

void RectTex::Move (float deltaX, float deltaY, float deltaZ)
{
	Sprite::Move (deltaX, deltaY, deltaZ);
	m_rectFigure.Move (deltaX, deltaY);
}

bool RectTex::CheckContainCursor (float coorX, float coorY)
{
	return m_rectFigure.IsContain (coorX, coorY);
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

	m_rectFigure.m_coorX  = -0.5f;
	m_rectFigure.m_coorY  =  0.5f;
	m_rectFigure.m_width  =  1.0f;
	m_rectFigure.m_height =  1.0f;
}

// RectFigure --------------------------------------------------------------------------------

RectFigure::RectFigure () :
	RectFigure (0, 0, 0, 0)
{}

RectFigure::RectFigure (float coorX, float coorY, float width, float height) :
	m_coorX (coorX), m_coorY (coorY), m_width (width), m_height (height)
{}

bool RectFigure::IsContain (MousePosition mousePosition)
{
	return mousePosition.x >= m_coorX && mousePosition.x <= m_coorX + m_width &&
		   mousePosition.y <= m_coorY && mousePosition.y >= m_coorY - m_width;
}

bool RectFigure::IsContain (float coorX, float coorY)
{
	return coorX >= m_coorX && coorX <= m_coorX + m_width &&
		   coorY <= m_coorY && coorY >= m_coorY - m_height;
}

void RectFigure::ScaleUp (float scaleX, float scaleY)
{
	m_coorX += m_width  * (1 - scaleX) / 2;
	m_coorY -= m_height * (1 - scaleY) / 2;
	m_width  *= scaleX;
	m_height *= scaleY;
}

void RectFigure::Move (float deltaX, float deltaY)
{
	m_coorX += deltaX;
	m_coorY += deltaY;
}

void RectFigure::Transpose ()
{
	std::swap (m_coorX, m_coorY);
	std::swap (m_height, m_width);
}
