#pragma once

#include <vector>

#include "Widget.h"

class WinMgr : public Widget
{
public:

	WinMgr (WinMgr *parent);
	virtual ~WinMgr ();

	virtual void Draw ();
	virtual void Update ();
	virtual void HandleNews (News news);
	
	void AddChildWidget (Widget *childWidget);
	void GetStartCoord (int *x, int *y);

	WinMgr &operator += (Widget *childWidget);

private:
	int m_coorStartX = 0;
	int m_coorStartY = 0;

	WinMgr *m_parent;
	std::vector <Widget *> m_childWidgets;
};