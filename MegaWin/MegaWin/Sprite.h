#pragma once

#include "VertexBuffer.h"
#include "Texture.h"
#include "DebugFuncs.h"

template <typename VertexT>
class Sprite
{
public:
	Sprite ();
	Sprite (VertexBuffer <VertexT> *vertexBuffer, Shader *shader, Texture *texture);
	virtual ~Sprite ();

	virtual bool Initialize (VertexBuffer <VertexT> *vertexBuffer,
							 Shader *shader, Texture *texture);

	virtual void Render (ID3D11DeviceContext *deviceContext);

protected:
	VertexBuffer <VertexT> *m_vertexBuffer;
	Texture *m_texture;
	Shader *m_shader;
};

template <typename VertexT>
Sprite <VertexT>::Sprite () :
	Sprite (nullptr, nullptr, nullptr)
{}

template <typename VertexT>
Sprite <VertexT>::Sprite (VertexBuffer <VertexT> *vertexBuffer, Shader *shader, Texture *texture) :
	m_vertexBuffer (vertexBuffer),
	m_shader (shader),
	m_texture (texture)
{}

template <typename VertexT>
Sprite <VertexT>::~Sprite ()
{
	if (m_vertexBuffer != nullptr)
	{
		delete m_vertexBuffer;
	}
}

template <typename VertexT>
bool Sprite <VertexT>::Initialize (VertexBuffer <VertexT> *vertexBuffer, Shader *shader, Texture *texture)
{
	CHECK_NULLPTR (vertexBuffer);
	CHECK_NULLPTR (shader);
	CHECK_NULLPTR (texture);

	m_vertexBuffer = vertexBuffer;
	m_shader = shader;
	m_texture = texture;

	return true;
}

template <typename VertexT>
void Sprite <VertexT>::Render (ID3D11DeviceContext *deviceContext)
{
	m_shader->SetShader (deviceContext);
	m_vertexBuffer->Render (deviceContext);
}