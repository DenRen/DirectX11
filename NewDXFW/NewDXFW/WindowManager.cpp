#include "WindowManager.h"
#include "RectTexButton.h"
#include "Config.h"
#include "InputCoorMouse.h"
#include "ScrollerHorizontal.h"

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

	/*
	auto buttonDarkTheme = new RectTexButton (0.7, 0.7 * WndCnf::maxY, 0.1, 0.1,
											  "Texture\\WidgetDarkTheme_Wait.png",
											  "Texture\\WidgetDarkTheme_Focused.png",
											  "Texture\\WidgetDarkTheme_Clicked.png");

	WinMgr::AddChildWidget (buttonDarkTheme);
	*/

	ScrollerHorizontal::InitDefTex_SurfaceButton	 ("Texture\\WidgetWait.png",
													  "Texture\\WidgetFocused.png",
													  "Texture\\WidgetClicked.png");

	ScrollerHorizontal::InitDefTex_FirstButtonArrow  ("Texture\\WidgetScrollerArrowLeft_Wait.png",
													  "Texture\\WidgetScrollerArrowLeft_Focused.png",
													  "Texture\\WidgetScrollerArrowLeft_Clicked.png");

	ScrollerHorizontal::InitDefTex_SecondButtonArrow ("Texture\\WidgetScrollerArrowRight_Wait.png",
													  "Texture\\WidgetScrollerArrowRight_Focused.png",
													  "Texture\\WidgetScrollerArrowRight_Clicked.png");

	ScrollerHorizontal::InitDefTex_ScrollSlider		 ("Texture\\WidgetScrollerSlider_Wait.png",
													  "Texture\\WidgetScrollerSlider_Focused.png",
													  "Texture\\WidgetScrollerSlider_Clicked.png");
	
	auto scroller = new ScrollerHorizontal (-0.75, -0.4, 1.5f, 0.1, 0.15);

	WinMgr::AddChildWidget (scroller);

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
