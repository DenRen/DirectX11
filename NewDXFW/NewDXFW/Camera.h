#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Structure_Data.h"

bool CreateConstatntBufferMatrixes (ID3D11Device *device, ID3D11Buffer **CBMatrixes);

class Camera
{
public:

	Camera (XMMATRIX view, XMMATRIX proj, WVPMatrixes *WVPMatrixes, ID3D11Buffer *CBMatrixes);

	void Render (ID3D11DeviceContext *deviceContext);

public:
	
	XMMATRIX m_view;
	XMMATRIX m_proj;

	WVPMatrixes *m_WVPMatrixes;
	ID3D11Buffer *m_CBWVPMatrixes;
};

