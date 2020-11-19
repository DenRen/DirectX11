#include "WindowManager.h"
#include "RectTexButton.h"
#include "Config.h"

WindowManager::WindowManager () :
	WinMgr (this),
	m_newsQueue (NewsQueue::GetNewsQueue ()),
	m_backGround (nullptr)
{}

bool WindowManager::Initialize ()
{
	RectTexButton::InitializeDefValues ("Texture\\WidgetWait.png",
										"Texture\\WidgetFocused.png",
										"Texture\\WidgetClicked.png");
	m_backGround = new RectTex (WndCnf::minX, WndCnf::maxY, WndCnf::lenX, WndCnf::lenY,
								"Texture\\Desktop.png");
	//m_backGround = new RectTex (-1.0f, 9.0f / 16.0, 2.0f, 9.0f / 8.0f, "Texture\\Desktop.png");

	auto buttonOK = new RectTexButton (-0.5, 0.5, 0.3, 0.1);

	AddChildWidget (buttonOK);

	return true;
}

void WindowManager::Draw ()
{
	m_backGround->Draw ();
	WinMgr::Draw ();
}

void WindowManager::Update ()
{
	WinMgr::Update ();
}
