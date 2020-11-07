#include <string>
#include "Texture.h"
#include "AddFunc.h"

Texture::Texture () :
	m_texture (nullptr),
	m_width (0),
	m_height (0)
{}

Texture::~Texture ()
{
	RELEASE (m_texture);
}

bool Texture::Initialize (ID3D11Device *device, LPCSTR fileName)
{
	m_name = std::string (fileName);

	HRESULT result = S_OK;

	// Load the texture
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

		m_width = desc.Width;
		m_height = desc.Height;

		resource->Release ();

		return true;
	}
	else
	{
		m_texture->Release ();
		resource->Release ();

		RETURN_FALSE;
	}
}

ID3D11ShaderResourceView *Texture::GetTexture ()
{
	return m_texture;
}

int Texture::GetWidth ()
{
	return m_width;
}

int Texture::GetHeight ()
{
	return m_height;
}
