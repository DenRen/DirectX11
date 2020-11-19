#include "WindowManager.h"
#include "RectTexButton.h"
#include "Config.h"
#include "InputCoorMouse.h"

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
	if (InputCoorMouse::Changed ())
	{
		News newsMouseCoor ((uint16_t) SENDER_NEWS::WINAPIWNDPROC);
		newsMouseCoor.m_news = NEWS::MOUSEMOVE;
		newsMouseCoor.m_mousePos = InputCoorMouse::GetPosition ();

		printf ("%f %f\n", newsMouseCoor.m_mousePos.x, newsMouseCoor.m_mousePos.y);
		WinMgr::HandleNews (newsMouseCoor);
	}
	
	int curSize = m_newsQueue->GetSize ();
	while (curSize--)
	{
		WinMgr::HandleNews (m_newsQueue->GetNews ());
	}

	WinMgr::Update ();
}
