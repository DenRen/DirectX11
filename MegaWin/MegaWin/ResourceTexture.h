#pragma once

#include <string>
#include "Texture.h"

class ResourceTexture
{
public:
	ResourceTexture ();
	~ResourceTexture ();

	bool Load (ID3D11Device *device, const char *textureFileName);

	Texture *GetTexture ();
	std::string GetName ();

private:
	Texture *m_texture;
	std::string m_name;
};

