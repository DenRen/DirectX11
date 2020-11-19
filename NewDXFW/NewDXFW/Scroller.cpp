#include "Scroller.h"

Scroller::Scroller () :
	m_stateSlider (0),
	m_firstButtonArrow		(nullptr),
	m_secondButtonArrow		(nullptr),
	m_scrollSlider			(nullptr),
	m_firstSurfaceButton	(nullptr),
	m_secondSurfaceButton	(nullptr),
	WinMgr (this)
{}
