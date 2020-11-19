#pragma once
#include "Button.h"
#include "Rectangle.h"

class RectTexButton : public Button, public RectTex
{
public:
	static void InitializeDefValues (const char *pathTextureWait,
									 const char *pathTextureFocus,
									 const char *pathTextureClicked);

	RectTexButton (float coorX, float coorY, float width, float height,
				   Texture *textureWait    = m_defTextureWait,
				   Texture *textureFocus   = m_defTextureFocused,
				   Texture *textureClicked = m_defTextureClicked);

	RectTexButton (float coorX, float coorY, float width, float height,
				   const char *pathTextureWait,
				   const char *pathTextureFocus,
				   const char *pathTextureClicked);

	void Draw ();
	void Update ();
	void HandleNews (News news);

private:
	
	float coorX;
	float coorY;

	void CheckContainCursor (MousePosition mousePosition);

	Texture *m_textureWait;
	Texture *m_textureFocused;
	Texture *m_textureClicked;

	static Texture *m_defTextureWait;
	static Texture *m_defTextureFocused;
	static Texture *m_defTextureClicked;
};

