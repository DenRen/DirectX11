#include <string>
#include "Texture.h"
#include "DebugFuncs.h"

Texture::Texture () :
	m_texture (nullptr),
	m_startSlot (0),
	m_sampler (nullptr),
	m_width  (0),
	m_height (0)
{}

Texture::~Texture ()
{
	RELEASE (m_texture);
}

bool Texture::Initialize (ID3D11Device *device, LPCSTR fileName, int startSlot)
{
	m_startSlot = startSlot;

	HRESULT result = S_OK;

	result = D3DX11CreateShaderResourceViewFromFile (device, fileName, nullptr, nullptr,
													 &m_texture, nullptr);
	CHECK_FAILED (result);

	// Get width and height
	ID3D11Resource *resource = nullptr;
	m_texture->GetResource (&resource);

	ID3D11Texture2D *texture2D = nullptr;
	result = resource->QueryInterface (&texture2D);

	if (SUCCEEDED (result))
	{
		D3D11_TEXTURE2D_DESC desc;
		texture2D->GetDesc (&desc);

		m_width  = desc.Width;
		m_height = desc.Height;

		resource->Release ();
	}
	else
	{
		m_texture->Release ();
		resource->Release ();

		RETURN_FALSE;
	}

	// Init sampler --------------------------------------------------------------------------

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	result = device->CreateSamplerState (&sampDesc, &m_sampler);

	if (FAILED (result))
	{
		RETURN_FALSE;
	}

	return true;
}

ID3D11ShaderResourceView *Texture::GetTexture ()
{
	return m_texture;
}

void Texture::Render (ID3D11DeviceContext *deviceContext)
{
	deviceContext->PSSetShaderResources (m_startSlot, 1, &m_texture);
	deviceContext->PSSetSamplers (m_startSlot, 1, &m_sampler);
}

int Texture::GetWidth ()
{
	return m_width;
}
int Texture::GetHeight ()
{
	return m_height;
}