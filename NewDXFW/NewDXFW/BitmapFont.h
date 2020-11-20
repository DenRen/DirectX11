#pragma once

#include <d3d11.h>
#include <d3dx11.h>
#include <vector>
#include <string>
#include <map>
#include <xnamath.h>

struct VertexFont
{
	XMFLOAT3 pos;
	XMFLOAT2 tex;
};

wchar_t *CharToWChar (char *mbString);

class BitmapFont
{
private:
	struct CharDesc
	{
		CharDesc () : srcX (0), srcY (0), srcW (0), srcH (0), xOff (0), yOff (0), xAdv (0) {}

		short srcX;
		short srcY;
		short srcW;
		short srcH;
		short xOff;
		short yOff;
		short xAdv;
	};

	struct ConstantBuffer
	{
		XMMATRIX WVP;
	};
	struct PixelBufferType
	{
		XMFLOAT4 pixelColor;
	};
public:
	BitmapFont ();

	bool Init (const char *fontFilename, const char *fontTexFile);
	void Render (unsigned int index, float r, float g, float b, float x, float y);
	void BuildVertexArray (VertexFont *vertices, const wchar_t *sentence, int screenWidth, int screenHeight);
	void Close ();

	ID3D11ShaderResourceView *GetTexture () { return m_texture; }

private:
	bool m_parse (const char *fontFilename);
	bool m_InitShader (const char *, const char *);
	void m_SetShaderParameters (float r, float g, float b, float x, float y);
	void m_RenderShader (unsigned int index);

	ID3D11Buffer *m_constantBuffer;
	ID3D11Buffer *m_pixelBuffer;
	ID3D11VertexShader *m_vertexShader;
	ID3D11PixelShader *m_pixelShader;
	ID3D11InputLayout *m_layout;
	ID3D11SamplerState *m_sampleState;
	ID3D11ShaderResourceView *m_texture;

	unsigned short m_WidthTex;
	unsigned short m_HeightTex;
	std::wstring m_file;
	std::map <int, CharDesc> m_Chars;
};