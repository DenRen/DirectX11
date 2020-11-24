#pragma once
#include "WinMgr.h"
#include "Rectangle.h"

class WindowManager : public WinMgr
{
public:
	WindowManager ();
	bool Initialize ();
	
	void Draw ();
	void Update ();
	
private:
	NewsQueue *m_newsQueue;
	RectTex *m_backGround;
};