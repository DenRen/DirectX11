#include "WinMgr.h"

WinMgr::WinMgr (WinMgr *parent, float startCoorX, float startCoorY) :
	m_parent (parent),
	m_startCoorX (startCoorX),
	m_startCoorY (startCoorY)
{}

WinMgr::~WinMgr ()
{
	int size = m_childWidgets.size ();
	while (size--)
	{
		delete m_childWidgets[size];
	}
}

void WinMgr::Draw ()
{
	int size = m_childWidgets.size ();
	while (size--)
	{
		auto child = m_childWidgets[size];
		if (child != nullptr)
		{
			child->Draw ();
		}
	}
}

void WinMgr::Update ()
{
	int size = m_childWidgets.size ();
	while (size--)
	{
		auto child = m_childWidgets[size];
		if (child != nullptr)
		{
			child->Update ();
		}
	}
}

void WinMgr::HandleNews (News news)
{
	int size = m_childWidgets.size ();
	while (size--)
	{
		auto child = m_childWidgets[size];
		if (child != nullptr)
		{
			child->HandleNews (news);
		}
	}
}

void WinMgr::InitStartCoord (float x, float y)
{
	m_startCoorX = x;
	m_startCoorY = y;
}

void WinMgr::AddChildWidget (Widget *childWidget)
{
	m_childWidgets.push_back (childWidget);
}

void WinMgr::DeleteAllChilds ()
{
	int size = m_childWidgets.size ();
	while (size--)
	{
		delete m_childWidgets[size];
	}
}

void WinMgr::GetStartCoord (float *x, float *y)
{
	*x = m_startCoorX;
	*y = m_startCoorY;
}

WinMgr &WinMgr::operator+=(Widget *childWidget)
{
	AddChildWidget (childWidget);
	
	return *this;
}
