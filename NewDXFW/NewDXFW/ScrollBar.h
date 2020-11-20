#pragma once

#include "RectTexButtonText.h"
#include <experimental/filesystem>

#include "ScrollerVertical.h"
#include "WinMgr.h"

namespace fs = std::experimental::filesystem;

class ScrollBar : public WinMgr
{
public:
	ScrollBar (float coorX, float coorY, float width, float height,
			   float widthScroll, float relHeightSlider, int capacity);

	void Draw ();
	void Update ();
	void HandleNews (News news);

	void Update (std::vector <std::string> names);
	void Clear ();

private:

	int m_capacity;
	int m_numFirst;
	int m_numActive;

	float m_width = 0.0f;
	float m_height = 0.0f;
	float m_widthScroll = 0.0f;

	std::vector <std::string> m_names;

	ScrollerVertical *m_scroller;
	RectTex *m_backGround;
	std::vector <RectTexButtonText *> m_bar;
	std::vector <int> m_idItemBar;
};

