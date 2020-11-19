#include "RectTexButton.h"
#include "ResourceManager.h"

TextureButton RectTexButton::def_textureButton (nullptr, nullptr, nullptr);

void RectTexButton::InitializeDefValues (const char *pathTextureWait,
										 const char *pathTextureFocus,
										 const char *pathTextureClicked)
{
	def_textureButton.m_wait	= ResMgr::GetResMgr ()->GetTexture (pathTextureWait);
	def_textureButton.m_focused = ResMgr::GetResMgr ()->GetTexture (pathTextureFocus);
	def_textureButton.m_clicked = ResMgr::GetResMgr ()->GetTexture (pathTextureClicked);
}

RectTexButton::RectTexButton (float coorX, float coorY, float width, float height,
							  const char *pathTextureWait,
							  const char *pathTextureFocus,
							  const char *pathTextureClicked) :
	RectTexButton (coorX, coorY, width, height,
				   TextureButton (ResMgr::GetResMgr ()->GetTexture (pathTextureWait),
								  ResMgr::GetResMgr ()->GetTexture (pathTextureFocus),
								  ResMgr::GetResMgr ()->GetTexture (pathTextureClicked)))
{}

RectTexButton::RectTexButton (float coorX, float coorY, float width, float height,
							  TextureButton textureButton) :
	RectTex (coorX, coorY, width, height),
	m_textureButton (textureButton)
{
	m_texture = textureButton.m_wait;
}

void RectTexButton::Draw ()
{
	RectTex::Draw ();
}

void RectTexButton::Update ()
{
	switch (Button::GetCurrentState ())
	{
	case BUTTONSTATE::WAIT:
		Sprite::SetTexture (m_textureButton.m_wait);
		break;
	case BUTTONSTATE::FOCUSED:
		Sprite::SetTexture (m_textureButton.m_focused);
		break;
	case BUTTONSTATE::CLICKED:
		Sprite::SetTexture (m_textureButton.m_clicked);
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
			case NEWS::LBUTTONDBLCLK:
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

TextureButton::TextureButton () :
	TextureButton (nullptr, nullptr, nullptr)
{}

TextureButton::TextureButton (Texture *textureWait,
							  Texture *textureFocused,
							  Texture *textureClicked) :
	m_wait (textureWait),
	m_focused (textureFocused),
	m_clicked (textureClicked)
{}
