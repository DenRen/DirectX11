#pragma once

#include <d3d11.h>
#include <xnamath.h>

#include "Structure_Data.h"

bool CreateConstatntBufferMatrixes (ID3D11Device *device, ID3D11Buffer **CBMatrixes);

class Camera
{
public:

	Camera (XMMATRIX view, XMMATRIX projection, ID3D11Buffer *CBMatrixes);

	void Render (ID3D11DeviceContext *deviceContext);

public:
	//ConstantBufferMatrixes m_matrix;
	XMMATRIX m_view;
	XMMATRIX m_proj;
	ID3D11Buffer *m_CBMatrixes;
};

