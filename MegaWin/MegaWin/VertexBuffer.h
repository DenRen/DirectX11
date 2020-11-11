#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Shader.h"
#include "DebugFuncs.h"
#include "AddFunc.h"

struct VertexPosTex
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

template <typename VertexT>
class VertexBuffer				// Base class
{
public:

	VertexBuffer ();
	~VertexBuffer ();

	template <typename IndexT>
	bool Initialize (ID3D11Device *device,
					 VertexT *vertices, int numberVertex,
					 IndexT  *indices,  int numberInidex,
					 ID3D11InputLayout *inputLayout);

	void Render (ID3D11DeviceContext *deviceContext, bool draw = true);
	void UpdateVertex (ID3D11DeviceContext *deviceContext);

	VertexT *GetVertices ();

	ID3D11Buffer *GetVertexBuffer ();
	ID3D11Buffer *GetIndexBuffer ();

	int GetVertexCount ();
	int GetIndexCount ();

private:
	ID3D11InputLayout *m_inputLayout;

	VertexT *m_vertices;

	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;

	int m_vertexCount;
	int m_indexCount;
};

template <typename VertexT>
VertexBuffer <VertexT>::VertexBuffer () :
	m_vertices		(nullptr),
	m_vertexBuffer	(nullptr),
	m_indexBuffer	(nullptr),
	m_vertexCount	(0),
	m_indexCount	(0)
{}

template <typename VertexT>
template <typename IndexT>
bool VertexBuffer<VertexT>::Initialize (ID3D11Device *device,
										VertexT *vertices, int numberVertex,
										IndexT  *indices,  int numberIndex,
										ID3D11InputLayout *inputLayout)
{
	m_inputLayout = inputLayout;

	m_vertexCount = numberVertex;
	m_indexCount  = numberIndex;

	D3D11_BUFFER_DESC bufDesc = {0};
	bufDesc.Usage = D3D11_USAGE_DEFAULT;
	bufDesc.ByteWidth = sizeof (VertexT) * m_vertexCount;
	bufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {0};
	ZeroMemory (&InitData, sizeof (InitData));
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

template <typename VertexT>
void VertexBuffer <VertexT>::Render (ID3D11DeviceContext *deviceContext, bool draw)
{
	unsigned int offset = 0;
	unsigned int stride = sizeof (VertexT);

	deviceContext->IASetVertexBuffers (0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer (m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->IASetInputLayout (m_inputLayout);

	if (draw)
	{
		deviceContext->DrawIndexed (m_indexCount, 0, 0);
	}
}

template <typename VertexT>
inline void VertexBuffer <VertexT>::UpdateVertex (ID3D11DeviceContext *deviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource = {};
	HRESULT result = deviceContext->Map (m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD,
										 0, &mappedResource);
	if (FAILED (result))
	{
		throw std::runtime_error ("");
	}

	VertexT *buffer = (VertexT *) mappedResource.pData;
	memcpy (buffer, m_vertices, sizeof (VertexT) * m_vertexCount);

	deviceContext->Unmap (m_vertexBuffer);
}

template<typename VertexT>
inline VertexT *VertexBuffer<VertexT>::GetVertices ()
{
	return m_vertices;
}

template <typename VertexT>
ID3D11Buffer *VertexBuffer <VertexT>::GetVertexBuffer ()
{
	return m_vertexBuffer;
}

template<typename VertexT>
inline ID3D11Buffer *VertexBuffer<VertexT>::GetIndexBuffer ()
{
	return m_indexBuffer;
}

template <typename VertexT>
int VertexBuffer <VertexT>::GetVertexCount ()
{
	return m_vertexCount;
}

template <typename VertexT>
int VertexBuffer <VertexT>::GetIndexCount ()
{
	return m_indexCount;
}

template <typename VertexT>
VertexBuffer <VertexT>::~VertexBuffer ()
{
	RELEASE (m_vertexBuffer);
	RELEASE (m_indexBuffer);
}