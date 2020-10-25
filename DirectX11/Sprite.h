#pragma once

#include "VertexBuffer.h"
#include "Texture.h"

class Sprite
{
public:
	Sprite (float size);
	virtual ~Sprite ();

	virtual bool Initialize (ID3D11Device *device, Shader *shader, LPCTSTR textureFileName);
	virtual void Update () {}
	virtual void Render (ID3D11DeviceContext *deviceContext, D3DXMATRIX worldMatrix,
						 D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix);

protected:
	VertexBuffer *m_vertexBuffer;
	Texture *m_texture;
	Shader *m_shader;
	float m_size;
};

