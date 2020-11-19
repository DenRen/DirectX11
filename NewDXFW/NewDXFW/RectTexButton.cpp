#include "RectTexButton.h"
#include "ResourceManager.h"

Texture *RectTexButton::m_defTextureWait	= nullptr;
Texture *RectTexButton::m_defTextureFocused	= nullptr;
Texture *RectTexButton::m_defTextureClicked = nullptr;

void RectTexButton::InitializeDefValues (const char *pathTextureWait,
										 const char *pathTextureFocus,
										 const char *pathTextureClicked)
{
	m_defTextureWait	= ResMgr::GetResMgr ()->GetTexture (pathTextureWait);
	m_defTextureFocused = ResMgr::GetResMgr ()->GetTexture (pathTextureFocus);
	m_defTextureClicked = ResMgr::GetResMgr ()->GetTexture (pathTextureClicked);
}

RectTexButton::RectTexButton (float coorX, float coorY, float width, float height,
							  Texture *textureWait,
							  Texture *textureFocus,
							  Texture *textureClicked) :
	RectTex (coorX, coorY, width, height),
	m_textureWait	 (textureWait),
	m_textureFocused   (textureFocus),
	m_textureClicked (textureClicked)
{
	m_texture = m_textureWait;
}

RectTexButton::RectTexButton (float coorX, float coorY, float width, float height,
							  const char *pathTextureWait,
							  const char *pathTextureFocus,
							  const char *pathTextureClicked) :
	RectTexButton (coorX, coorY, width, height,
				   ResMgr::GetResMgr ()->GetTexture (pathTextureWait),
				   ResMgr::GetResMgr ()->GetTexture (pathTextureFocus),
				   ResMgr::GetResMgr ()->GetTexture (pathTextureClicked))
{}

void RectTexButton::Draw ()
{
	RectTex::Draw ();
}

void RectTexButton::Update ()
{
	switch (Button::GetCurrentState ())
	{
	case BUTTONSTATE::WAIT:
		Sprite::SetTexture (m_textureWait);
		break;
	case BUTTONSTATE::FOCUSED:
		Sprite::SetTexture (m_textureFocused);
		break;
	case BUTTONSTATE::CLICKED:
		Sprite::SetTexture (m_textureClicked);
		break;
	}
}

void RectTexButton::HandleNews (News news)
{
	// Вообще здесь можно сделать общий стандарт для любой кнопки
	// Приходит news, запомниаем необходимые данные и выставляем радные флаги.
	// Затем в Update () эти флаги и данные обрабатываем. Именно там отправляем новые news
	// и устанавливаем новые текстуры, например. Но это обобщение для для общих методов.

	if (news.m_idSender == (uint16_t) SENDER_NEWS::WINAPIWNDPROC)
	{
		printf ("%f %f\n", news.m_mousePos.x, news.m_mousePos.y);
		if (news.m_news >= NEWS::MOUSEFIRST && news.m_news <= NEWS::MOUSELAST)
		{
			Button::SetStateWait ();
			switch (news.m_news)
			{
			case NEWS::MOUSEMOVE:
				{
					CheckContainCursor (news.m_mousePos);
				} break;
			case NEWS::LBUTTONDOWN:
				{
					CheckContainCursor (news.m_mousePos);
					if (Button::GetCurrentState () == BUTTONSTATE::FOCUSED)
					{
						Button::SetStateClicked ();
					}
				}
			}
		}
	}
}

void RectTexButton::CheckContainCursor (MousePosition mousePosition)
{
	if (RectTex::CheckContainCursor (mousePosition.x, mousePosition.y))
	{
		Button::SetStateFocused ();
	}
}