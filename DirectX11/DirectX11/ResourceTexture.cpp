#include "ResourceTexture.h"

ResourceTexture::ResourceTexture () :
	m_texture (nullptr)
{}

ResourceTexture::~ResourceTexture ()
{
	if (m_texture != nullptr)
	{
		delete m_texture;
	}
}

bool ResourceTexture::Load (ID3D11Device *device, const char *textureFileName)
{
	if (m_texture != nullptr)
	{
		m_name.erase ();
		delete m_texture;
	}

	m_name = std::string (textureFileName);
	m_texture = new Texture ();
	if (!m_texture->Initialize (device, textureFileName))
	{
		delete m_texture;
		m_texture = nullptr;

		return false;
	}

	return true;
}

Texture *ResourceTexture::GetTexture ()
{
	return m_texture;
}

std::string ResourceTexture::GetName ()
{
	return m_name;
}
