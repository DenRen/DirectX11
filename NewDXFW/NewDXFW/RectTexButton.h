#pragma once
#include "Button.h"
#include "Rectangle.h"

struct TextureButton
{
	TextureButton ();
	TextureButton (const TextureButton &) = default;
	TextureButton (Texture *m_textureWait, Texture *m_textureFocused, Texture *m_textureClicked);

	Texture *m_wait;
	Texture *m_focused;
	Texture *m_clicked;
};

class RectTexButton : public Button, public RectTex
{
public:
	static void InitializeDefValues (const char *pathTextureWait,
									 const char *pathTextureFocus,
									 const char *pathTextureClicked);

	RectTexButton (float coorX, float coorY, float width, float height,
				   TextureButton textureButton = def_textureButton);

	RectTexButton (float coorX, float coorY, float width, float height,
				   const char *pathTextureWait,
				   const char *pathTextureFocus,
				   const char *pathTextureClicked);

	void Draw ();
	void Update ();
	void HandleNews (News news);

private:

	void CheckContainCursor (MousePosition mousePosition);

	TextureButton m_textureButton;

	static TextureButton def_textureButton;
};

