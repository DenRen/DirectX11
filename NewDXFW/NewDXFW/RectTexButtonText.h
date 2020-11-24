#pragma once

#include "RectTexButton.h"
#include "Text.h"

class RectTexButtonText : public RectTexButton
{
public:

	RectTexButtonText (float coorX, float coorY, float width, float height, const char *text);

	static bool InitDefFont (const char *pathFontFile, const char *pathFontTex);

	void Draw ();
	void Update ();
	void HandleNews (News news);

private:
	Text *m_text;

	static BitmapFont *def_font;
};

