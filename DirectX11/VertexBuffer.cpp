#include "VertexBuffer.h"
#include "AddFunc.h"

VertexBuffer::VertexBuffer () :
	m_vertices (nullptr),
	m_vertexBuffer (nullptr),
	m_indexBuffer (nullptr),
	m_shader (nullptr)
{}

VertexBuffer::~VertexBuffer ()
{
	if (m_vertices != nullptr)
	{
		delete [] m_vertices;
		m_vertices = nullptr;
	}

	RELEASE (m_vertexBuffer);
	RELEASE (m_indexBuffer);
}

bool VertexBuffer::Initialize (ID3D11Device *device, Shader *shader, float size, bool writeable)
{
	m_shader = shader;

	unsigned long *indices = nullptr;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result = S_OK;

	m_vertexCount = 4;
	m_indexCount = 6;

	m_vertices = new VertexType[m_vertexCount];
	indices = new unsigned long[m_indexCount];

	float halfSize = size / 2.0;

	// Load vertex array
	m_vertices[0].position = D3DXVECTOR3 (-halfSize, -halfSize, 0.0f);
	m_vertices[0].uv	   = D3DXVECTOR2 (0.0f, 1.0f);

	m_vertices[1].position = D3DXVECTOR3 (-halfSize, +halfSize, 0.0f);
	m_vertices[1].uv	   = D3DXVECTOR2 (0.0f, 0.0f);

	m_vertices[2].position = D3DXVECTOR3 (+halfSize, +halfSize, 0.0f);
	m_vertices[2].uv	   = D3DXVECTOR2 (1.0f, 0.0f);

	m_vertices[3].position = D3DXVECTOR3 (+halfSize, -halfSize, 0.0f);
	m_vertices[3].uv	   = D3DXVECTOR2 (1.0f, 1.0f);

	// Load indices
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	// Set up vertex buffer description
	vertexBufferDesc.Usage				 = (writeable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth			 = sizeof (VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags			 = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags		 = (writeable) ? D3D11_CPU_ACCESS_WRITE : NULL;
	vertexBufferDesc.MiscFlags			 = NULL;
	vertexBufferDesc.StructureByteStride = NULL;

	// Set up vertex data
	vertexData.pSysMem = m_vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	result = device->CreateBuffer (&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	CHECK_FAILED (result);

	// Set up index buffer description
	indexBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth			= sizeof (unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags		= NULL;
	indexBufferDesc.MiscFlags			= NULL;
	indexBufferDesc.StructureByteStride = NULL;

	// Set up index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create index buffer
	result = device->CreateBuffer (&indexBufferDesc, &indexData, &m_indexBuffer);
	CHECK_FAILED (result);

	// Release
	delete [] indices;
	indices = nullptr;

	return true;
}

void VertexBuffer::Render (ID3D11DeviceContext *deviceContext)
{
	unsigned int offset = 0;
	unsigned int stride = sizeof (VertexType);

	m_shader->Begin (deviceContext, m_indexCount);

	deviceContext->IASetVertexBuffers (0, 1, &m_vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer (m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_shader->End (deviceContext);
}

VertexBuffer::VertexType *VertexBuffer::GetVertices ()
{
	return m_vertices;
}

ID3D11Buffer *VertexBuffer::GetVertexBuffer ()
{
	return m_vertexBuffer;
}

int VertexBuffer::GetIndexCount ()
{
	return m_indexCount;
}
