#include "ScrollBar.h"

ScrollBar::ScrollBar (float coorX, float coorY, float width, float height,
					  float widthScroll, float relHeightSlider, int capacity) :
	m_scroller (new ScrollerVertical (coorX, coorY, width, height, relHeightSlider)),
	m_capacity (capacity)
{}
