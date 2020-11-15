#pragma once

#include <D3DX11async.h>

class InputLayout
{
public:
	virtual D3D11_INPUT_ELEMENT_DESC *GetLayout ()
	{
		return nullptr;
	};

	virtual int GetNumElements ()
	{
		return -1;
	}
};