#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Shader.h"
#include "DebugFunc.h"
#include "AddFunc.h"
#include "InputLayout.h"

struct VertexPosTex : public InputLayout
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;

	VertexPosTex () :
		VertexPosTex (0, 0)
	{}

	VertexPosTex (XMFLOAT3 pos, XMFLOAT2 tex) :
		pos (pos),
		tex (tex)
	{}

	static D3D11_INPUT_ELEMENT_DESC *GetLayout ()
	{
		int size = 0;
		static D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		return layout;
	}

	static int GetNumElements ()
	{
		return 2;
	};
};

template <typename VertexT, typename IndexT>
class VertexBuffer
{
public:

	VertexBuffer ();
	~VertexBuffer ();

	bool Initialize (ID3D11Device *device,
					 VertexT *vertices, int numberVertex,
					 IndexT  *indices,  int numberInidex);

	void Render (ID3D11DeviceContext *deviceContext);
	void UpdateVertex (ID3D11DeviceContext *deviceContext, VertexT *vertices);

	ID3D11Buffer *GetVertexBuffer ();
	ID3D11Buffer *GetIndexBuffer ();

	int GetVertexCount ();
	int GetIndexCount ();

private:
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;

	int m_vertexCount;
	int m_indexCount;
};

template <typename VertexT, typename IndexT>
VertexBuffer <VertexT, IndexT>::VertexBuffer () :
	m_vertexBuffer (nullptr),
	m_indexBuffer  (nullptr),
	m_vertexCount  (0),
	m_indexCount   (0)
{}

template <typename VertexT, typename IndexT>
bool VertexBuffer<VertexT, IndexT>::Initialize (ID3D11Device *device,
												VertexT *vertices, int numberVertex,
												IndexT  *indices,  int numberIndex)
{
	m_vertexCount = numberVertex;
	m_indexCount  = numberIndex;

	D3D11_BUFFER_DESC bufDesc = {};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof (VertexT) * m_vertexCount;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	SET_IN_ZERO (InitData);
	InitData.pSysMem = vertices;

	HRESULT result = device->CreateBuffer (&bufDesc, &InitData, &m_vertexBuffer);
	if (FAILED (result))
	{
		RETURN_FALSE;
	}

	// ------------------------------------------------------------------------------------------

	SET_IN_ZERO (bufDesc);
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof (IndexT) * m_indexCount;
	bufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;

	InitData.pSysMem = indices;

	result = device->CreateBuffer (&bufDesc, &InitData, &m_indexBuffer);
	if (FAILED (result))
	{
		RETURN_FALSE;
	}

	return true;
}

template <typename VertexT, typename IndexT>
void VertexBuffer <VertexT, IndexT>::Render (ID3D11DeviceContext *deviceContext)
{
	unsigned int offset = 0;
	unsigned int stride = sizeof (VertexT);

	deviceContext->IASetVertexBuffers (0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer (m_indexBuffer, DXGI_FORMAT_R8_UINT, 0);
	deviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->DrawIndexed (m_indexCount, 0, 0);
}

template <typename VertexT, typename IndexT>
inline void VertexBuffer <VertexT, IndexT>::UpdateVertex (ID3D11DeviceContext *deviceContext,
														  VertexT *vertices)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT result = deviceContext->Map (m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,
										 0, &mappedResource);
	if (FAILED (result))
	{
		throw std::runtime_error ("");
	}

	VertexT *buffer = (VertexT *) mappedResource.pData;
	memcpy (buffer, vertices, sizeof (VertexT) * m_vertexCount);

	deviceContext->Unmap (m_vertexBuffer);
}

template <typename VertexT, typename IndexT>
ID3D11Buffer *VertexBuffer <VertexT, IndexT>::GetVertexBuffer ()
{
	return m_vertexBuffer;
}

template<typename VertexT, typename IndexT>
inline ID3D11Buffer *VertexBuffer<VertexT, IndexT>::GetIndexBuffer ()
{
	return m_indexBuffer;
}

template <typename VertexT, typename IndexT>
int VertexBuffer <VertexT, IndexT>::GetVertexCount ()
{
	return m_vertexCount;
}

template <typename VertexT, typename IndexT>
int VertexBuffer <VertexT, IndexT>::GetIndexCount ()
{
	return m_indexCount;
}

template <typename VertexT, typename IndexT>
VertexBuffer <VertexT, IndexT>::~VertexBuffer ()
{
	RELEASE (m_vertexBuffer);
	RELEASE (m_indexBuffer);
}