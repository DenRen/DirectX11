#pragma once

#include <string>
#include "BitmapFont.h"
#include "DXManager.h"

class Text
{
public:
	Text (BitmapFont *font);

	bool Init (const std::wstring &text, int scrW = 1280, int scrH = 720);
	void Render (float r, float g, float b, float x, float y);
	void Close ();

private:
	bool m_InitBuffers (const std::wstring &text, int scrW, int scrH);
	void m_RenderBuffers ();

	BitmapFont *m_font;
	ID3D11Buffer *m_vertexBuffer;
	ID3D11Buffer *m_indexBuffer;
	int m_numindex;
};