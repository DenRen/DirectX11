#include "ScrollerHorizontal.h"
#include "ResourceManager.h"

TextureButton ScrollerHorizontal::def_texFirstButtonArrow;
TextureButton ScrollerHorizontal::def_texSecondButtonArrow;
TextureButton ScrollerHorizontal::def_texScrollSlider;
TextureButton ScrollerHorizontal::def_texSurfaceButton;

ScrollerHorizontal::ScrollerHorizontal (float coorX, float coorY, float width, float height,
										float relWidthScroll)
{
	MYASSERT (relWidthScroll <= 1.0f);

	m_stateSlider = relWidthScroll / 2;
	m_widthSurface = width - 2 * height;
	m_relWidthSlider = relWidthScroll;

	m_firstButtonArrow   = new RectTexButton (coorX,				coorY, height,	height,
											  def_texFirstButtonArrow);
	coorX += height;

	m_secondButtonArrow  = new RectTexButton (coorX + m_widthSurface, coorY, height,	height,
											  def_texSecondButtonArrow);


	m_scrollSlider		 = new RectTexButton (coorX, coorY, m_widthSurface * relWidthScroll, height,
											  def_texScrollSlider);

	m_firstSurfaceButton = new RectTexButton (coorX, coorY, m_widthSurface * relWidthScroll / 2,   height,
											  def_texSurfaceButton);

	m_secondSurfaceButton = new RectTexButton (coorX + m_widthSurface * relWidthScroll / 2, coorY, m_widthSurface * (1 - relWidthScroll / 2), height,
											   def_texSurfaceButton);

	WinMgr::AddChildWidget (m_scrollSlider);
	WinMgr::AddChildWidget (m_firstButtonArrow);
	WinMgr::AddChildWidget (m_secondButtonArrow);
	WinMgr::AddChildWidget (m_firstSurfaceButton);
	WinMgr::AddChildWidget (m_secondSurfaceButton);
}

void ScrollerHorizontal::InitDefTex_FirstButtonArrow (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texFirstButtonArrow.m_wait	  = mgr->GetTexture (wait);
	def_texFirstButtonArrow.m_focused = mgr->GetTexture (focused);
	def_texFirstButtonArrow.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerHorizontal::InitDefTex_SecondButtonArrow (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texSecondButtonArrow.m_wait    = mgr->GetTexture (wait);
	def_texSecondButtonArrow.m_focused = mgr->GetTexture (focused);
	def_texSecondButtonArrow.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerHorizontal::InitDefTex_ScrollSlider (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texScrollSlider.m_wait    = mgr->GetTexture (wait);
	def_texScrollSlider.m_focused = mgr->GetTexture (focused);
	def_texScrollSlider.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerHorizontal::InitDefTex_SurfaceButton (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texSurfaceButton.m_wait    = mgr->GetTexture (wait);
	def_texSurfaceButton.m_focused = mgr->GetTexture (focused);
	def_texSurfaceButton.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerHorizontal::SetSlider (float state)
{
	if (state <= -0.0001)
	{
		state = 0.0f;
	}
	else if (state >= 1.0001)
	{
		state = 1.0f;
	}

	if (state <=     m_relWidthSlider / 2) state =        m_relWidthSlider / 2;
	if (state >= 1 - m_relWidthSlider / 2) state = 1.0f - m_relWidthSlider / 2;

	//state = m_relWidthSlider / 2 + (1 - m_relWidthSlider) * state;

	float deltaX = 0.5 * m_widthSurface * ((double) state - m_stateSlider);

	m_firstSurfaceButton->ScaleUp (state / m_stateSlider, 1.0f, 1.0f);
	m_firstSurfaceButton->Move (deltaX, 0.0f, 0.0f);

	m_secondSurfaceButton->ScaleUp (1/ ((1.0f - m_stateSlider) / (1.0f - state)), 1.0f, 1.0f);
	m_secondSurfaceButton->Move (deltaX, 0.0f, 0.0f);

	m_scrollSlider->Move (2 * deltaX, 0.0f, 0.0f);

	m_stateSlider = state;
}

void ScrollerHorizontal::MoveSlider (float deltaState)
{
	SetSlider (m_stateSlider + deltaState / (m_widthSurface * (1 - m_relWidthSlider)));
}

void ScrollerHorizontal::Draw ()
{
	WinMgr::Draw ();
}

void ScrollerHorizontal::Update ()
{
	const float m_deltaX = 0.1;

	if (m_scrollSlider->IsPassed ())
	{
		printf ("------------------------------\n");
		MoveSlider (m_deltaMousePosition.x);
	}

	if (m_firstButtonArrow->HaveSingleClick ()   ) SetSlider (m_stateSlider - m_deltaX);
	if (m_secondButtonArrow->HaveSingleClick ()  ) SetSlider (m_stateSlider + m_deltaX);
	if (m_firstSurfaceButton->HaveSingleClick () ) SetSlider (m_stateSlider - m_deltaX);
	if (m_secondSurfaceButton->HaveSingleClick ()) SetSlider (m_stateSlider + m_deltaX);
	
	m_prevScrollClicked = m_scrollSlider->GetCurrentState () == BUTTONSTATE::CLICKED;

	WinMgr::Update ();
}

void ScrollerHorizontal::HandleNews (News news)
{
	if (news.m_idSender == (uint16_t) SENDER_NEWS::WINAPIWNDPROC)
	{
		if (news.m_news == NEWS::MOUSEMOVE)
		{
			m_deltaMousePosition.x = m_prevMousePosition.x - news.m_mousePos.x;
			m_deltaMousePosition.y = m_prevMousePosition.y - news.m_mousePos.y;
			
			printf ("%f %f\n", m_deltaMousePosition.x, m_deltaMousePosition.y);

			m_prevMousePosition.x = news.m_mousePos.x;
			m_prevMousePosition.y = news.m_mousePos.y;
		}
	}

	m_firstButtonArrow->HandleNews (news);
	m_secondButtonArrow->HandleNews (news);
	m_scrollSlider->HandleNews (news);

	if (m_scrollSlider->GetCurrentState () == BUTTONSTATE::WAIT)
	{
		m_firstSurfaceButton->HandleNews (news);
		m_secondSurfaceButton->HandleNews (news);
	}
	else
	{
		m_firstSurfaceButton->SetStateWait ();
		m_secondSurfaceButton->SetStateWait ();
	}
	
	//WinMgr::HandleNews (news);
}
