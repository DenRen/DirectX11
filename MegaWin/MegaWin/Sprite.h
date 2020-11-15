#pragma once

#include "VertexBuffer.h"
#include "Texture.h"
#include "DebugFuncs.h"

template <typename VertexT, typename IndexT>
class Sprite
{
public:
	Sprite ();
	Sprite (Shader *shader, Texture *texture);
	virtual ~Sprite ();

	virtual bool Initialize (Shader *shader, Texture *texture);

	virtual void Render (ID3D11DeviceContext *deviceContext);	// Automaticly draw

protected:
	VertexBuffer <VertexT, IndexT> m_vertexBuffer;
	Texture *m_texture;
	Shader *m_shader;
};

template <typename VertexT, typename IndexT>
Sprite <VertexT, IndexT>::Sprite () :
	Sprite (nullptr, nullptr)
{}

template <typename VertexT, typename IndexT>
Sprite <VertexT, IndexT>::Sprite (Shader *shader, Texture *texture) :
	m_shader	   (shader),
	m_texture	   (texture)
{}

template <typename VertexT, typename IndexT>
Sprite <VertexT, IndexT>::~Sprite ()
{

}

template <typename VertexT, typename IndexT>
bool Sprite <VertexT, IndexT>::Initialize (Shader *shader, Texture *texture)
{
	CHECK_NULLPTR (shader);
	CHECK_NULLPTR (texture);

	m_shader = shader;
	m_texture = texture;

	return true;
}

template <typename VertexT, typename IndexT>
void Sprite <VertexT, IndexT>::Render (ID3D11DeviceContext *deviceContext)
{
	m_texture->Render	  (deviceContext);
	m_shader->Render	  (deviceContext);
	m_vertexBuffer.Render (deviceContext);
}