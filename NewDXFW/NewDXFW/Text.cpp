#include "Text.h"
#include "DebugFunc.h"

Text::Text (BitmapFont *font)
{
	m_font = font;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_numindex = 0;
}

bool Text::Init (const std::wstring &text, int screenWidth, int screenHeight)
{
	if (!m_InitBuffers (text, screenWidth, screenHeight))
		return false;

	return true;
}

bool Text::m_InitBuffers (const std::wstring &text, int screenWidth, int screenHeight)
{
	int vertnum = text.size () * 6;
	VertexFont *vertex = new VertexFont[vertnum];
	if (!vertex)
		return false;

	m_font->BuildVertexArray (vertex, text.c_str (), screenWidth, screenHeight);

	m_numindex = text.size () * 6;
	unsigned long *indices = new unsigned long[m_numindex];

	for (int i = 0; i < m_numindex; i++)
		indices[i] = i;

	HRESULT result;
	D3D11_BUFFER_DESC BufferDesc;
	D3D11_SUBRESOURCE_DATA Data;

	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof (VertexFont) * vertnum;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	Data.pSysMem = vertex;
	Data.SysMemPitch = 0;
	Data.SysMemSlicePitch = 0;

	result = DXManager::GetDevice ()->CreateBuffer (&BufferDesc, &Data, &m_vertexBuffer);
	if (FAILED (result))
		return false;

	BufferDesc.ByteWidth = sizeof (unsigned long) * m_numindex;
	BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	Data.pSysMem = indices;

	result = DXManager::GetDevice ()->CreateBuffer (&BufferDesc, &Data, &m_indexBuffer);
	if (FAILED (result))
		return false;

	delete [] vertex;
	delete [] indices;

	return true;
}

void Text::Render (float r, float g, float b, float x, float y)
{
	m_RenderBuffers ();
	m_font->Render (m_numindex, r, g, b, x, -y);
}

void Text::Close ()
{
	RELEASE (m_vertexBuffer);
	RELEASE (m_indexBuffer);
}

void Text::m_RenderBuffers ()
{
	unsigned int stride = sizeof (VertexFont);
	unsigned int offset = 0;

	auto devCont = DXManager::GetDeviceContext ();
	devCont->IASetVertexBuffers (0, 1, &m_vertexBuffer, &stride, &offset);
	devCont->IASetIndexBuffer (m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devCont->IASetPrimitiveTopology (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}