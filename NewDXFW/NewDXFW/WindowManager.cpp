#include "WindowManager.h"
#include "RectTexButton.h"
#include "Config.h"
#include "InputCoorMouse.h"
#include "ScrollerHorizontal.h"
#include "ScrollerVertical.h"

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

	// Horizontal default values --------------------------------------------------------------------------

	ScrollerHorizontal::InitDefTex_SurfaceButton	 ("Texture\\H\\WidgetWait.png",
													  "Texture\\H\\WidgetFocused.png",
													  "Texture\\H\\WidgetClicked.png");

	ScrollerHorizontal::InitDefTex_FirstButtonArrow  ("Texture\\H\\WidgetScrollerArrowLeft_Wait.png",
													  "Texture\\H\\WidgetScrollerArrowLeft_Focused.png",
													  "Texture\\H\\WidgetScrollerArrowLeft_Clicked.png");

	ScrollerHorizontal::InitDefTex_SecondButtonArrow ("Texture\\H\\WidgetScrollerArrowRight_Wait.png",
													  "Texture\\H\\WidgetScrollerArrowRight_Focused.png",
													  "Texture\\H\\WidgetScrollerArrowRight_Clicked.png");

	ScrollerHorizontal::InitDefTex_ScrollSlider		 ("Texture\\H\\WidgetScrollerSlider_Wait.png",
													  "Texture\\H\\WidgetScrollerSlider_Focused.png",
													  "Texture\\H\\WidgetScrollerSlider_Clicked.png");
	
	// Vertical default values ----------------------------------------------------------------------------

	ScrollerVertical::InitDefTex_SurfaceButton     ("Texture\\V\\WidgetWait.png",
												    "Texture\\V\\WidgetFocused.png",
												    "Texture\\V\\WidgetClicked.png");

	ScrollerVertical::InitDefTex_FirstButtonArrow  ("Texture\\V\\WidgetScrollerArrowLeft_Wait.png",
												    "Texture\\V\\WidgetScrollerArrowLeft_Focused.png",
												    "Texture\\V\\WidgetScrollerArrowLeft_Clicked.png");

	ScrollerVertical::InitDefTex_SecondButtonArrow ("Texture\\V\\WidgetScrollerArrowRight_Wait.png",
													"Texture\\V\\WidgetScrollerArrowRight_Focused.png",
													"Texture\\V\\WidgetScrollerArrowRight_Clicked.png");

	ScrollerVertical::InitDefTex_ScrollSlider	   ("Texture\\V\\WidgetScrollerSlider_Wait.png",
												    "Texture\\V\\WidgetScrollerSlider_Focused.png",
												    "Texture\\V\\WidgetScrollerSlider_Clicked.png");

	// -------------------------------------------------------------------------------------------------

	auto scrollerV = new ScrollerVertical   ( 0.75, +0.4, 0.5, 0.1, 0.3);
	auto scrollerH = new ScrollerHorizontal (-0.75, -0.4, 1.5, 0.1, 0.15);

	WinMgr::AddChildWidget (scrollerH);
	WinMgr::AddChildWidget (scrollerV);

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

		//printf ("%f %f\n", newsMouseCoor.m_mousePos.x, newsMouseCoor.m_mousePos.y);
		WinMgr::HandleNews (newsMouseCoor);
	}
	
	int curSize = m_newsQueue->GetSize ();
	while (curSize--)
	{
		WinMgr::HandleNews (m_newsQueue->GetNews ());
	}

	WinMgr::Update ();
}
