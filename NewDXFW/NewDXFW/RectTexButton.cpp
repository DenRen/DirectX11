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

}

void RectTexButton::HandleNews (News news)
{
	// Вообще здесь можно сделать общий стандарт для любой кнопки
	// Приходит news, запомниаем необходимые данные и выставляем радные флаги.
	// Затем в Update () эти флаги и данные обрабатываем. Именно там отправляем новые news
	// и устанавливаем новые текстуры, например. Но это обобщение для для общих методов.


}

void RectTexButton::SetStateWait ()
{
	Button::SetStateWait ();
	Sprite::SetTexture (m_textureWait);
}

void RectTexButton::SetStateFocused ()
{
	Button::SetStateFocused ();
	Sprite::SetTexture (m_textureFocused);
}

void RectTexButton::SetStateClicked ()
{
	Button::SetStateClicked ();
	Sprite::SetTexture (m_textureClicked);
}
