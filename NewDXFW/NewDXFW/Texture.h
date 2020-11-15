#pragma once

#include <d3d11.h>
#include <D3DX11tex.h>

class Texture
{
public:
	Texture ();
	~Texture ();

	bool Initialize (ID3D11Device *device, LPCSTR fileName, int startSlot = 0);

	ID3D11ShaderResourceView *GetTexture ();

	void Render (ID3D11DeviceContext *deviceContext);

	int GetWidth ();
	int GetHeight ();

private:
	ID3D11ShaderResourceView *m_texture;
	int m_startSlot;

	ID3D11SamplerState *m_sampler;

	int m_width;
	int m_height;
};

