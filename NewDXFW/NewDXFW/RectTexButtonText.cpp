#include "RectTexButtonText.h"

BitmapFont *RectTexButtonText::def_font = new BitmapFont ();

RectTexButtonText::RectTexButtonText (float coorX, float coorY, float width, float height,
									  const char *text) :
	RectTexButton (coorX, coorY, width, height),
	m_text (new Text (def_font))
{
	m_text->Init (CharToWChar (const_cast <char *> (text)));
}

bool RectTexButtonText::InitDefFont (const char *pathFontFile, const char *pathFontTex)
{
	if (!def_font->Init (pathFontFile, pathFontTex))
	{
		RETURN_FALSE;
	}

	return true;
}

void RectTexButtonText::Draw ()
{
	RectTexButton::Draw ();
	auto rect = RectTexButton::GetRectFigure ();

	m_text->Render (0, 0, 0, rect.m_coorX + rect.m_width * 0.1, rect.m_coorY - rect.m_height / 6 );
}

void RectTexButtonText::Update ()
{
	RectTexButton::Update ();
}

void RectTexButtonText::HandleNews (News news)
{
	RectTexButton::HandleNews (news);
}
