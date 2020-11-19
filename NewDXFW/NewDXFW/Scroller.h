#pragma once

#include "WinMgr.h"
#include "RectTexButton.h"

class Scroller : public WinMgr
{
public:

	Scroller ();

protected:
	float m_stateSlider;
	float m_widthSurface;
	float m_relWidthSlider;

	RectTexButton *m_firstButtonArrow;
	RectTexButton *m_secondButtonArrow;
	RectTexButton *m_scrollSlider;
	RectTexButton *m_firstSurfaceButton;
	RectTexButton *m_secondSurfaceButton;
};

