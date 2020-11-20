#pragma once

#include "WinMgr.h"
#include "RectTexButton.h"

class Scroller : public WinMgr
{
public:

	Scroller ();

protected:
	float m_stateSlider;
	float m_heightSurface;
	float m_relHeightSlider;

	RectTexButton *m_firstButtonArrow;
	RectTexButton *m_secondButtonArrow;
	RectTexButton *m_scrollSlider;
	RectTexButton *m_firstSurfaceButton;
	RectTexButton *m_secondSurfaceButton;
};

