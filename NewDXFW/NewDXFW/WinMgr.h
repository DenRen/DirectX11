#pragma once

#include <vector>

#include "Widget.h"

class WinMgr : public Widget
{
public:

	WinMgr (WinMgr *parent, float startCoorX = 0.0f, float startCoorY = 0.0f);
	virtual ~WinMgr ();

	virtual void Draw ();
	virtual void Update ();
	virtual void HandleNews (News news);
	
	void InitStartCoord (float x, float y);
	void AddChildWidget (Widget *childWidget);
	void DeleteAllChilds ();
	void GetStartCoord (float*x, float *y);

	WinMgr &operator += (Widget *childWidget);

private:
	float m_startCoorX = 0;
	float m_startCoorY = 0;

	WinMgr *m_parent;
	std::vector <Widget *> m_childWidgets;
};