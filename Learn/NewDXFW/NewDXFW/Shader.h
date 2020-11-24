#pragma once

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include <string>
#include "Shaders.h"

class Shader	// S = PS + VS => P + V = 1 => P (sem_mutex) = 1 - V (sem_mutex)
{
public:
	Shader (VertexShader *vertexShader, PixelShader *pixelShader);

	virtual void Render    (ID3D11DeviceContext *deviceContext);
	virtual void RemShader (ID3D11DeviceContext *deviceContext);

	bool IsInitialized ();

//private:
	VertexShader *m_vertexShader;
	PixelShader  *m_pixelShader;
};