#include "ScrollerVertical.h"
#include "ResourceManager.h"

TextureButton ScrollerVertical::def_texFirstButtonArrow;
TextureButton ScrollerVertical::def_texSecondButtonArrow;
TextureButton ScrollerVertical::def_texScrollSlider;
TextureButton ScrollerVertical::def_texSurfaceButton;

ScrollerVertical::ScrollerVertical (float coorX, float coorY, float height, float width,
									float relHeightScroll)
{

	MYASSERT (relHeightScroll <= 1.0f);

	m_stateSlider = 0;
	m_heightSurface = height - 2 * width;
	m_relHeightSlider = relHeightScroll;

	m_firstButtonArrow = new RectTexButton (coorX, coorY, width, width,
											def_texFirstButtonArrow);
	coorY -= width;

	m_secondButtonArrow = new RectTexButton (coorX, coorY - m_heightSurface, width, width,
											 def_texSecondButtonArrow);


	m_scrollSlider = new RectTexButton (coorX, coorY, width, m_heightSurface * relHeightScroll,
										def_texScrollSlider);

	m_firstSurfaceButton = new RectTexButton (coorX, coorY, width, m_heightSurface * relHeightScroll / 2,
											  def_texSurfaceButton);

	m_secondSurfaceButton = new RectTexButton (coorX, coorY - m_heightSurface * relHeightScroll / 2, width, m_heightSurface * (1 - relHeightScroll / 2),
											   def_texSurfaceButton);

	WinMgr::AddChildWidget (m_scrollSlider);
	WinMgr::AddChildWidget (m_firstButtonArrow);
	WinMgr::AddChildWidget (m_secondButtonArrow);
	WinMgr::AddChildWidget (m_firstSurfaceButton);
	WinMgr::AddChildWidget (m_secondSurfaceButton);
}

void ScrollerVertical::InitDefTex_FirstButtonArrow (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texFirstButtonArrow.m_wait = mgr->GetTexture (wait);
	def_texFirstButtonArrow.m_focused = mgr->GetTexture (focused);
	def_texFirstButtonArrow.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerVertical::InitDefTex_SecondButtonArrow (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texSecondButtonArrow.m_wait = mgr->GetTexture (wait);
	def_texSecondButtonArrow.m_focused = mgr->GetTexture (focused);
	def_texSecondButtonArrow.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerVertical::InitDefTex_ScrollSlider (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texScrollSlider.m_wait = mgr->GetTexture (wait);
	def_texScrollSlider.m_focused = mgr->GetTexture (focused);
	def_texScrollSlider.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerVertical::InitDefTex_SurfaceButton (const char *wait, const char *focused, const char *clicked)
{
	auto mgr = ResMgr::GetResMgr ();
	def_texSurfaceButton.m_wait = mgr->GetTexture (wait);
	def_texSurfaceButton.m_focused = mgr->GetTexture (focused);
	def_texSurfaceButton.m_clicked = mgr->GetTexture (clicked);
}

void ScrollerVertical::SetSlider (float state)
{
	if (state <= -0.0001)
	{
		state = 0.0f;
	}
	else if (state >= 1.0001)
	{
		state = 1.0f;
	}

	//if (state <=     m_relHeightSlider / 2) state =        m_relHeightSlider / 2;
	//if (state >= 1 - m_relHeightSlider / 2) state = 1.0f - m_relHeightSlider / 2;

	float saveState = state;
	state = m_relHeightSlider / 2 + (1 - m_relHeightSlider) * state;
	float stateOld = m_relHeightSlider / 2 + (1 - m_relHeightSlider) * m_stateSlider;

	float deltaY = 0.5 * m_heightSurface * ((double)state - stateOld);

	m_firstSurfaceButton->ScaleUp (1.0f, state / stateOld, 1.0f);
	m_firstSurfaceButton->Move (0.0f, -deltaY, 0.0f);

	m_secondSurfaceButton->ScaleUp (1.0f, 1 / ((1.0f - stateOld) / (1.0f - state)), 1.0f);
	m_secondSurfaceButton->Move (0.0f, -deltaY, 0.0f);

	m_scrollSlider->Move (0.0f, -2 * deltaY, 0.0f);

	m_stateSlider = saveState;
}

void ScrollerVertical::MoveSlider (float deltaState)
{
	SetSlider (m_stateSlider + deltaState / (m_heightSurface * (1 - m_relHeightSlider)));
}

float ScrollerVertical::GetStateSlider ()
{
	return m_stateSlider;
}

void ScrollerVertical::Draw ()
{
	WinMgr::Draw ();
}

void ScrollerVertical::Update ()
{
	const float m_deltaX = 0.1;

	if (m_scrollSlider->HaveSingleClick ())
	{
		printf ("------------------------------\n");
		MoveSlider (m_deltaMousePosition.x);
	}

	if (m_firstButtonArrow->HaveSingleClick ())    SetSlider (m_stateSlider - m_deltaX);
	if (m_secondButtonArrow->HaveSingleClick ())   SetSlider (m_stateSlider + m_deltaX);
	if (m_firstSurfaceButton->HaveSingleClick ())  SetSlider (m_stateSlider - m_deltaX);
	if (m_secondSurfaceButton->HaveSingleClick ()) SetSlider (m_stateSlider + m_deltaX);

	m_prevScrollClicked = m_scrollSlider->GetCurrentState () == BUTTONSTATE::CLICKED;

	WinMgr::Update ();
}

void ScrollerVertical::HandleNews (News news)
{
	if (news.m_idSender == (uint16_t)SENDER_NEWS::WINAPIWNDPROC)
	{
		if (news.m_news == NEWS::MOUSEMOVE)
		{
			m_deltaMousePosition.x = m_prevMousePosition.x - news.m_mousePos.x;
			m_deltaMousePosition.y = m_prevMousePosition.y - news.m_mousePos.y;

			//if (fabs (m_deltaMousePosition.x) > 0.001 && fabs (m_deltaMousePosition.x) > 0.001)
				//printf ("%f %f\n", m_deltaMousePosition.x, m_deltaMousePosition.y);

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
