#pragma once

#include <d3d11.h>
#include <D3DX10math.h>

#include "Shader.h"

class VertexBuffer
{
public:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 uv;
	};

	VertexBuffer ();
	~VertexBuffer ();

	bool Initialize (ID3D11Device *device, Shader *shader, float size, bool writeable = true);
	void Render (ID3D11DeviceContext *deviceContext);

	VertexType *GetVertices ();
	ID3D11Buffer *GetVertexBuffer ();
	int GetVertexCount ();
	int GetIndexCount ();

private:
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	VertexType *m_vertices;
	int m_vertexCount;
	int m_indexCount;
	Shader *m_shader;
};

