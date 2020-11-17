#pragma once

#include <d3d11.h>
#include <xnamath.h>

struct WVPMatrixes
{
	WVPMatrixes () = default;
	WVPMatrixes (const WVPMatrixes &) = default;
	WVPMatrixes (XMMATRIX world, XMMATRIX view, XMMATRIX projection);

	void UpdateSubresource (ID3D11DeviceContext *deviceContext, ID3D11Buffer *CBWVPMatrixes);

	XMMATRIX m_World;
	XMMATRIX m_View;
	XMMATRIX m_Projection;
};

struct VertexPosTex
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
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, size,
															D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,	 0, size += sizeof (pos),
															D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		return layout;
	}

	static int GetNumElements ()
	{
		return 2;
	};
};

struct SimpleVertex
{
	XMFLOAT3 pos;

	SimpleVertex () :
		pos (0, 0, 0)
	{}

	SimpleVertex (XMFLOAT3 pos) :
		pos (pos)
	{}

	static D3D11_INPUT_ELEMENT_DESC *GetLayout ()
	{
		static D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		return layout;
	}

	static int GetNumElements ()
	{
		return 1;
	};
};