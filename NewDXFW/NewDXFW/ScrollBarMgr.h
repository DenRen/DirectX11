#pragma once

#include <experimental/filesystem>

#include "WinMgr.h"
#include "ScrollBar.h"

namespace fs = std::experimental::filesystem;

class ScrollBarMgr : public WinMgr
{
public:
	ScrollBarMgr (float coorX, float coorY, float width, float height,
				  float widthScroll, float relHeightSlider, int capacity,
				  fs::path initPath = fs::current_path ());

	void Draw ();
	void Update ();
	void HandleNews (News news);

private:
	fs::path m_curPath;
	ScrollBar *m_scrollBar;
};

