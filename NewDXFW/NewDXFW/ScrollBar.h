#pragma once

#include "RectTexButtonText.h"
#include "ScrollerVertical.h"
#include "WinMgr.h"

class ScrollBar : public WinMgr
{
public:
	ScrollBar (float coorX, float coorY, float width, float height,
			   float widthScroll, float relHeightSlider, int capacity);


private:
	int m_capacity;

	ScrollerVertical *m_scroller;
	std::vector <RectTexButtonText *> m_bar;
};

