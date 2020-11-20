#include "BitmapFont.h"

#include <fstream>
#include <string>
#include <sstream>

#include "DebugFunc.h"
#include "DXManager.h"
#include "Shaders.h"
#include "Config.h"

wchar_t *CharToWChar (char *mbString)
{
	int len = 0;
	len = (int) strlen (mbString) + 1;
	wchar_t *ucString = new wchar_t[len];
	mbstowcs (ucString, mbString, len);
	return ucString;
}

BitmapFont::BitmapFont ()
{
	m_texture = nullptr;
	m_constantBuffer = nullptr;
	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_layout = nullptr;
	m_sampleState = nullptr;
	m_pixelBuffer = nullptr;
	m_WidthTex = 0;
	m_HeightTex = 0;
}

bool BitmapFont::Init (const char *fontFilename, const char *fontTexFile)
{
	if (!m_parse (fontFilename))
		RETURN_FALSE;

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile (DXManager::GetDevice (),
														 fontTexFile, NULL, NULL,
														 &m_texture, NULL);
	CHECK_FAILED (hr);

	if (!m_InitShader ("Shader\\BitmapFont.vs", "Shader\\BitmapFont.ps"))
		RETURN_FALSE;

	return true;
}

bool BitmapFont::m_parse (const char *fontFilename)
{
	std::ifstream fin;
	fin.open (fontFilename);
	if (fin.fail ())
		return false;

	std::string Line;
	std::string Read, Key, Value;
	size_t i;
	while (!fin.eof ())
	{
		std::stringstream LineStream;
		std::getline (fin, Line);
		LineStream << Line;

		LineStream >> Read;
		if (Read == "common")
		{
			while (!LineStream.eof ())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find ('=');
				Key = Read.substr (0, i);
				Value = Read.substr (i + 1);

				Converter << Value;
				if (Key == "scaleW")
					Converter >> m_WidthTex;
				else if (Key == "scaleH")
					Converter >> m_HeightTex;
			}
		}
		else if (Read == "page")
		{
			while (!LineStream.eof ())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find ('=');
				Key = Read.substr (0, i);
				Value = Read.substr (i + 1);

				std::string str;
				Converter << Value;
				if (Key == "file")
				{
					Converter >> str;
					wchar_t *name = CharToWChar ((char *)str.substr (1, Value.length () - 2).c_str ());
					m_file = name;
					//_DELETE_ARRAY (name);
				}
			}
		}
		else if (Read == "char")
		{
			unsigned short CharID = 0;
			CharDesc chard;

			while (!LineStream.eof ())
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find ('=');
				Key = Read.substr (0, i);
				Value = Read.substr (i + 1);

				Converter << Value;
				if (Key == "id")
					Converter >> CharID;
				else if (Key == "x")
					Converter >> chard.srcX;
				else if (Key == "y")
					Converter >> chard.srcY;
				else if (Key == "width")
					Converter >> chard.srcW;
				else if (Key == "height")
					Converter >> chard.srcH;
				else if (Key == "xoffset")
					Converter >> chard.xOff;
				else if (Key == "yoffset")
					Converter >> chard.yOff;
				else if (Key == "xadvance")
					Converter >> chard.xAdv;
			}
			m_Chars.insert (std::pair<int, CharDesc> (CharID, chard));
		}
	}

	fin.close ();

	return true;
}

bool BitmapFont::m_InitShader (const char *vsFilename, const char *psFilename)
{
	ID3DBlob *vertexShaderBuffer = nullptr;

	HRESULT hr = CompileShaderFromFile (vsFilename, "VS", "vs_4_0", &vertexShaderBuffer);
	if (FAILED (hr))
		return false;

	ID3DBlob *pixelShaderBuffer = nullptr;
	HRESULT result = CompileShaderFromFile (psFilename, "PS", "ps_4_0", &pixelShaderBuffer);
	if (FAILED (hr))
		return false;

	result = DXManager::GetDevice ()->CreateVertexShader (vertexShaderBuffer->GetBufferPointer (), vertexShaderBuffer->GetBufferSize (), NULL, &m_vertexShader);
	if (FAILED (result))
		return false;

	result = DXManager::GetDevice ()->CreatePixelShader (pixelShaderBuffer->GetBufferPointer (), pixelShaderBuffer->GetBufferSize (), NULL, &m_pixelShader);
	if (FAILED (result))
		return false;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;
	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	unsigned int numElements = sizeof (polygonLayout) / sizeof (polygonLayout[0]);

	result = DXManager::GetDevice ()->CreateInputLayout (polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer (), vertexShaderBuffer->GetBufferSize (), &m_layout);
	if (FAILED (result))
		return false;

	RELEASE (vertexShaderBuffer);
	RELEASE (pixelShaderBuffer);

	D3D11_BUFFER_DESC BufferDesc;
	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof (ConstantBuffer);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	result = DXManager::GetDevice ()->CreateBuffer (&BufferDesc, NULL, &m_constantBuffer);
	if (FAILED (result))
		return false;

	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.ByteWidth = sizeof (PixelBufferType);
	BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferDesc.CPUAccessFlags = 0;
	BufferDesc.MiscFlags = 0;
	BufferDesc.StructureByteStride = 0;

	result = DXManager::GetDevice ()->CreateBuffer (&BufferDesc, NULL, &m_pixelBuffer);
	if (FAILED (result))
		return false;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = DXManager::GetDevice ()->CreateSamplerState (&samplerDesc, &m_sampleState);
	if (FAILED (result))
		return false;

	return true;
}

void BitmapFont::BuildVertexArray (VertexFont *vertices, const wchar_t *sentence,
								   int screenWidth, int screenHeight)
{
	int numLetters = (int) wcslen (sentence);

	float drawX0 = 0.0f;
	float drawY0 = 1.0 * m_Chars['A'].srcH / 2;

	float drawX = drawX0, drawY = drawY0;

	float offsetY = 0;
	float offsetX = 0;

	int index = 0;
	for (int i = 0; i < numLetters; i++)
	{
		if (sentence[i] == '\n')
		{
			offsetY += m_Chars['A'].srcH;
			drawX = drawX0;
		}

		float CharX = m_Chars[sentence[i]].srcX;
		float CharY = m_Chars[sentence[i]].srcY;
		float Width = m_Chars[sentence[i]].srcW;
		float Height = m_Chars[sentence[i]].srcH;
		float OffsetX = m_Chars[sentence[i]].xOff;
		float OffsetY = m_Chars[sentence[i]].yOff + offsetY;

		float left = drawX + OffsetX;
		float right = left + Width;
		float top = drawY - OffsetY;
		float bottom = top - Height;
		float lefttex = CharX / m_WidthTex;
		float righttex = (CharX + Width) / m_WidthTex;
		float toptex = CharY / m_HeightTex;
		float bottomtex = (CharY + Height) / m_HeightTex;

		vertices[index].pos = XMFLOAT3 (left, top, 0.0f);
		vertices[index].tex = XMFLOAT2 (lefttex, toptex);
		index++;
		vertices[index].pos = XMFLOAT3 (right, bottom, 0.0f);
		vertices[index].tex = XMFLOAT2 (righttex, bottomtex);
		index++;
		vertices[index].pos = XMFLOAT3 (left, bottom, 0.0f);
		vertices[index].tex = XMFLOAT2 (lefttex, bottomtex);
		index++;
		vertices[index].pos = XMFLOAT3 (left, top, 0.0f);
		vertices[index].tex = XMFLOAT2 (lefttex, toptex);
		index++;
		vertices[index].pos = XMFLOAT3 (right, top, 0.0f);
		vertices[index].tex = XMFLOAT2 (righttex, toptex);
		index++;
		vertices[index].pos = XMFLOAT3 (right, bottom, 0.0f);
		vertices[index].tex = XMFLOAT2 (righttex, bottomtex);
		index++;

		drawX += m_Chars[sentence[i]].xAdv;
	}
}

void BitmapFont::Render (unsigned int index, float r, float g, float b, float x, float y)
{
	m_SetShaderParameters (r, g, b, x, y);
	m_RenderShader (index);
}

void BitmapFont::m_SetShaderParameters (float r, float g, float b, float x, float y)
{
	x *= WndCnf::WIDTH  / WndCnf::lenX;
	y *= WndCnf::HEIGHT / WndCnf::lenY;

	XMMATRIX objmatrix = XMMatrixScaling (1.0f, 1, 1) * XMMatrixTranslation (x, -y, 0);
	XMMATRIX wvp = objmatrix * XMMatrixOrthographicLH (WndCnf::WIDTH, WndCnf::HEIGHT, 0.0f, 100.0f);
	ConstantBuffer cb;
	cb.WVP = XMMatrixTranspose (wvp);

	auto devCont = DXManager::GetDeviceContext ();

	devCont->UpdateSubresource (m_constantBuffer, 0, NULL, &cb, 0, 0);
	
	devCont->VSSetConstantBuffers (0, 1, &m_constantBuffer);
	
	devCont->PSSetShaderResources (0, 1, &m_texture);

	PixelBufferType pb;
	pb.pixelColor = XMFLOAT4 (r, g, b, 1.0f);
	devCont->UpdateSubresource (m_pixelBuffer, 0, NULL, &pb, 0, 0);
	
	devCont->PSSetConstantBuffers (0, 1, &m_pixelBuffer);
}

void BitmapFont::m_RenderShader (unsigned int index)
{
	auto devCont = DXManager::GetDeviceContext ();

	devCont->IASetInputLayout (m_layout);
	devCont->VSSetShader (m_vertexShader, NULL, 0);
	devCont->PSSetShader (m_pixelShader, NULL, 0);
	devCont->PSSetSamplers (0, 1, &m_sampleState);
	devCont->DrawIndexed (index, 0, 0);
}

void BitmapFont::Close ()
{
	RELEASE (m_constantBuffer);
	RELEASE (m_pixelBuffer);
	RELEASE (m_vertexShader);
	RELEASE (m_pixelShader);
	RELEASE (m_layout);
	RELEASE (m_sampleState);
	RELEASE (m_texture);
}