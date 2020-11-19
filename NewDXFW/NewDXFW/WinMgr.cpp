#include "WinMgr.h"

WinMgr::WinMgr (WinMgr *parent) :
	m_parent (parent)
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
			child->HandleNews(news);
		}
	}
}

void WinMgr::AddChildWidget (Widget *childWidget)
{
	m_childWidgets.push_back (childWidget);
}

void WinMgr::GetStartCoord (int *x, int *y)
{
	*x = m_coorStartX;
	*y = m_coorStartY;
}

WinMgr &WinMgr::operator+=(Widget *childWidget)
{
	AddChildWidget (childWidget);
	
	return *this;
}
