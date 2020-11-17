#pragma once

#include "VertexBuffer.h"
#include "Texture.h"
#include "DXManager.h"
#include "DebugFunc.h"
#include "Structure_Data.h"

template <typename VertexT, typename IndexT>
class Sprite
{
public:
	Sprite ();
	Sprite (Shader *shader, Texture *texture,
			ID3D11Buffer *CBMatrix, WVPMatrixes *WVPMatrixes,
			XMMATRIX world = XMMatrixIdentity ());

	bool Initialize (Shader *shader, Texture *texture);

	void Render (ID3D11DeviceContext *deviceContext);	// Automaticly draw

	void Move (float x, float y, float z);
	void RotateX (float angle);
	void RotateZ (float angle);
	void RotateY (float angle);
	void ScaleUp (float ScaleUpX, float ScaleUpY, float ScaleUpZ);

protected:
	VertexBuffer <VertexT, IndexT> m_vertexBuffer;
	Texture *m_texture;
	Shader  *m_shader;

	XMMATRIX m_worldMatrix;
	WVPMatrixes *m_WVPMatrixes;
	ID3D11Buffer *m_CBWVPMatrixes;

private:
	void SetCBMatrix ();
};

template <typename VertexT, typename IndexT>
inline Sprite <VertexT, IndexT>::Sprite () :
	Sprite (nullptr, nullptr, nullptr, nullptr, XMMatrixIdentity ())
{}

template <typename VertexT, typename IndexT>
inline Sprite <VertexT, IndexT>::Sprite (Shader *shader, Texture *texture,
										 ID3D11Buffer *CBWVPMatrix, WVPMatrixes *WVPMatrixes,
										 XMMATRIX world) :
	m_shader (shader),
	m_texture (texture),
	m_CBWVPMatrixes (CBWVPMatrix),
	m_WVPMatrixes (WVPMatrixes),
	m_worldMatrix (world)
{}

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
	m_texture->Render (deviceContext);
	m_shader->Render (deviceContext);
	m_vertexBuffer.Render (deviceContext);

	SetCBMatrix ();
}

template <typename VertexT, typename IndexT>
void Sprite <VertexT, IndexT>::SetCBMatrix ()
{
	ID3D11DeviceContext *deviceContext = DXManager::GetDeviceContext ();

	m_WVPMatrixes->m_World = m_worldMatrix;
	m_WVPMatrixes->UpdateSubresource (deviceContext, m_CBWVPMatrixes);

	deviceContext->VSSetConstantBuffers (0, 1, &m_CBWVPMatrixes);
}

template <typename VertexT, typename IndexT>
void Sprite <VertexT, IndexT>::Move (float x, float y, float z)
{
	m_worldMatrix *= XMMatrixTranslation (x, y, z);
}

template<typename VertexT, typename IndexT>
inline void Sprite<VertexT, IndexT>::RotateX (float angle)
{
	m_worldMatrix *= XMMatrixRotationX (angle);
}

template<typename VertexT, typename IndexT>
inline void Sprite<VertexT, IndexT>::RotateY (float angle)
{
	m_worldMatrix *= XMMatrixRotationY (angle);
}

template<typename VertexT, typename IndexT>
inline void Sprite<VertexT, IndexT>::RotateZ (float angle)
{
	m_worldMatrix *= XMMatrixRotationZ (angle);
}

template<typename VertexT, typename IndexT>
inline void Sprite <VertexT, IndexT>::ScaleUp (float ScaleUpX, float ScaleUpY, float ScaleUpZ)
{
	m_worldMatrix *= XMMatrixScaling (ScaleUpX, ScaleUpY, ScaleUpZ);
}
